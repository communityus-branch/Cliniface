/************************************************************************
 * Copyright (C) 2018 Spatial Information Systems Research Limited
 *
 * Cliniface is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cliniface is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#ifndef Cliniface_CLINIFACE_MAIN_H
#define Cliniface_CLINIFACE_MAIN_H

#include <ActionCrop.h>
#include <ActionAddPath.h>
#include <ActionMarquee.h>
#include <ActionEditPaths.h>
#include <ActionExportPDF.h>
#include <ActionToggleAxes.h>
#include <ActionToggleFXAA.h>
#include <ActionDeletePath.h>
#include <ActionRenamePath.h>
#include <ActionSetOpacity.h>
#include <ActionResetCamera.h>
#include <ActionShowMetrics.h>
#include <ActionShowScanInfo.h>
#include <ActionRadialSelect.h>
#include <ActionEditLandmarks.h>
#include <ActionUpdateMetrics.h>
#include <ActionLoadFaceModels.h>
#include <ActionSaveScreenshot.h>
#include <ActionOrientCameraToFace.h>
#include <ActionToggleScalarLegend.h>
#include <ActionToggleStereoRendering.h>
#include <ActionSetParallelProjection.h>
#include <ActionSynchroniseCameraMovement.h>
#include <ActionToggleCameraActorInteraction.h>

#include <ModelEntryExitInteractor.h>
#include <ContextMenuInteractor.h>
#include <MetricsDialog.h>
#include <MultiFaceModelViewer.h>
#include <FaceActionManager.h>
#include <QDragEnterEvent>
#include <QMainWindow>

#include "ClinifacePluginsLoader.h"
#include "VisualisationsOrganiser.h"
#include <Cliniface_Config.h>
#include <lua.hpp>

using namespace FaceTools::Action;


namespace Ui { class ClinifaceMain;}

namespace Cliniface {

class ClinifaceMain : public QMainWindow
{ Q_OBJECT
public:
    ClinifaceMain();
    ~ClinifaceMain() override;

public slots:
    bool loadModel( const QString&);
    bool closeModel();  // Close the current model returning true if closed

protected:
    void dragEnterEvent( QDragEnterEvent*) override;
    void dropEvent( QDropEvent*) override;
    void closeEvent( QCloseEvent *) override;
    QSize sizeHint() const override;

private slots:
    void doOnUpdateSelected( FaceTools::FM*, bool);

private:
    Ui::ClinifaceMain *ui;
    FaceTools::FM *_cmodel;
    ClinifacePluginsLoader *_ploader;
    FaceActionManager *_fam;
    VisualisationsOrganiser *_vorg;
    FaceTools::MultiFaceModelViewer *_mfmv;
    FaceTools::Interactor::ModelEntryExitInteractor *_meei;
    FaceTools::Interactor::ContextMenuInteractor *_cmenu;

    FaceAction *_actionRemesh;
    FaceAction *_actionSmooth;
    FaceAction *_actionReflect;
    FaceAction *_actionSetFocus;
    FaceAction *_actionRotateX90;
    FaceAction *_actionRotateY90;
    FaceAction *_actionRotateZ90;
    FaceAction *_actionFillHoles;
    FaceAction *_actionDetectFace;
    FaceAction *_actionVisTexture;
    FaceAction *_actionVisOutlines;
    FaceAction *_actionVisWireframe;
    FaceAction *_actionGetComponent;
    FaceAction *_actionInvertNormals;
    FaceAction *_actionAlignLandmarks;
    FaceAction *_actionResetDetection;
    FaceAction *_actionSaveFaceModels;
    FaceAction *_actionBackfaceCulling;
    FaceAction *_actionCloseFaceModels;
    FaceAction *_actionSaveAsFaceModel;
    FaceAction *_actionLoadDirFaceModels;
    FaceAction *_actionCloseAllFaceModels;
    FaceAction *_actionOrientCameraToFrontFace;
    FaceAction *_actionTransformToStandardPosition;

    FaceAction *_actionSetMinScalarColour;
    FaceAction *_actionSetMaxScalarColour;
    FaceAction *_actionSetNumScalarColours;
    FaceAction *_actionChangeSurfaceMappingRange;

    ActionCrop                           *_actionCrop;
    ActionAddPath                        *_actionAddPath;
    ActionMarquee                        *_actionMarquee;
    ActionEditPaths                      *_actionEditPaths;
    ActionExportPDF                      *_actionExportPDF;
    ActionToggleAxes                     *_actionToggleAxes;
    ActionToggleFXAA                     *_actionToggleFXAA;
    ActionDeletePath                     *_actionDeletePath;
    ActionRenamePath                     *_actionRenamePath;
    ActionSetOpacity                     *_actionSetOpacity;
    ActionResetCamera                    *_actionResetCamera;
    ActionShowMetrics                    *_actionShowMetrics;
    ActionShowScanInfo                   *_actionShowScanInfo;
    ActionRadialSelect                   *_actionRadialSelect;
    ActionEditLandmarks                  *_actionEditLandmarks;
    ActionUpdateMetrics                  *_actionUpdateMetrics;
    ActionLoadFaceModels                 *_actionLoadFaceModels;
    ActionSaveScreenshot                 *_actionSaveScreenshot;
    ActionOrientCameraToFace             *_actionOrientCameraToLeftFace;
    ActionOrientCameraToFace             *_actionOrientCameraToRightFace;
    ActionToggleScalarLegend             *_actionToggleScalarLegend;
    ActionToggleStereoRendering          *_actionToggleStereoRendering;
    ActionSetParallelProjection          *_actionSetParallelProjection;
    ActionSynchroniseCameraMovement      *_actionSynchroniseCameraMovement;
    ActionToggleCameraActorInteraction   *_actionToggleCameraActorInteraction;

    void createFileMenu();
    void createViewMenu();
    void createToolsMenu();
    void createMetricsMenu();
    void createHelpMenu();
    void createToolBar();
    void createContextMenu();

    void createActions();
    void createMetrics();
    void registerActions();

    void setupMainViewer();

    ClinifaceMain( const ClinifaceMain&) = delete;
    void operator=( const ClinifaceMain&) = delete;
};  // end class

}   // end namespace

#endif
