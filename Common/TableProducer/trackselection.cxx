// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

//
// Task performing basic track selection.
//

#include "Framework/AnalysisDataModel.h"
#include "Framework/AnalysisTask.h"
#include "Framework/runDataProcessing.h"
#include "Common/Core/TrackSelection.h"
#include "Common/Core/TrackSelectionDefaults.h"
#include "Common/DataModel/TrackSelectionTables.h"
#include "Common/Core/trackUtilities.h"

using namespace o2;
using namespace o2::framework;
using namespace o2::framework::expressions;

//****************************************************************************************
/**
 * Produce track filter table.
 */
//****************************************************************************************
struct TrackSelectionTask {
  // FIXME: this will be removed once we can get this via meta data
  Configurable<bool> isRun3{"isRun3", false, "temp option to enable run3 mode"};

  Produces<aod::TrackSelection> filterTable;

  TrackSelection globalTracks;
  TrackSelection globalTracksSDD;

  void init(InitContext&)
  {
    globalTracks = getGlobalTrackSelection();
    globalTracksSDD = getGlobalTrackSelectionSDD();

    if (isRun3) {
      globalTracks.SetTrackType(o2::aod::track::TrackTypeEnum::Track);
    }
  }

  void process(soa::Join<aod::FullTracks, aod::TracksExtended> const& tracks)
  {
    for (auto& track : tracks) {
      filterTable((uint8_t)globalTracks.IsSelected(track),
                  (uint8_t)globalTracksSDD.IsSelected(track));
    }
  }
};

//****************************************************************************************
/**
 * Workflow definition.
 */
//****************************************************************************************
WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  WorkflowSpec workflow{adaptAnalysisTask<TrackSelectionTask>(cfgc, TaskName{"track-selection"})};
  return workflow;
}
