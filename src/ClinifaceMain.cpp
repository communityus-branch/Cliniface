#include <ClinifaceMain.h>
#include <ui_ClinifaceMain.h>

#include <AboutDialog.h>
#include <HelpDialog.h>
#include <LicensesDialog.h>

#include <FaceModelU3DFileHandler.h>
#include <FaceModelAssImpFileHandlerFactory.h>
#include <FaceModelOBJFileHandler.h>
#include <FaceModelXMLFileHandler.h>
#include <LandmarksVisualisation.h>
#include <BoundingVisualisation.h>
#include <OutlinesVisualisation.h>
#include <SampleReport.h>

#include <U3DExporter.h>    // RModelIO

#include <QLabel>
#include <QMimeData>
#include <QApplication>
#include <QDesktopWidget>
using Cliniface::ClinifaceMain;


namespace {
QAction* connectDialog( QAction* a, QDialog* d)
{
    QObject::connect( a, &QAction::triggered, d, &QDialog::show);
    QObject::connect( a, &QAction::triggered, d, &QDialog::raise);
    return a;
}   // end connectDialog
}   // end namespace


// private
void ClinifaceMain::createFileMenu()
{
    QList<QAction*> ioactions;
    ioactions << _actionLoadFaceModels->qaction()
              << _actionLoadDirFaceModels->qaction()
              << _actionSaveFaceModels->qaction()
              << _actionSaveAsFaceModel->qaction();

    if ( ActionExportPDF::isAvailable())
        ioactions << _actionExportPDF->qaction();

    ioactions << _actionCloseFaceModels->qaction()
              << _actionCloseAllFaceModels->qaction();

    ui->menu_File->addActions( ioactions);
    ui->menu_File->addSeparator();
    ui->menu_File->addAction( ui->action_Exit);
}   // end createFileMenu


// private
void ClinifaceMain::createViewMenu()
{
    /****** ui->menu_View ******/
    ui->menu_View->addAction( _actionOrientCameraToLeftFace->qaction());
    ui->menu_View->addAction( _actionOrientCameraToFrontFace->qaction());
    ui->menu_View->addAction( _actionOrientCameraToRightFace->qaction());
    ui->menu_View->addAction( _actionResetCamera->qaction());
    ui->menu_View->addAction( _actionSynchroniseCameraMovement->qaction());
    ui->menu_View->addAction( _actionToggleCameraActorInteraction->qaction());
    ui->menu_View->addSeparator();
    ui->menu_View->addActions( _fam->visualisations().actions());
    ui->menu_View->addAction( _actionRadialSelect->qaction());
    ui->menu_View->addSeparator();
    ui->menu_View->addAction( _actionMarquee->qaction());
    ui->menu_View->addAction( _actionBackfaceCulling->qaction());
    ui->menu_View->addAction( _actionSetParallelProjection->qaction());
    ui->menu_View->addAction( _actionToggleAxes->qaction());
    ui->menu_View->addAction( ui->action_ShowVisToolbar);
    ui->menu_View->addAction( _actionSaveScreenshot->qaction());

    /****** ui->visToolBar ******/
    ui->visToolBar->addActions( _fam->visualisations().actions());
    ui->visToolBar->addAction( _actionRadialSelect->qaction());
    ui->visToolBar->addSeparator();
    ui->visToolBar->addAction( _actionBackfaceCulling->qaction());
    ui->visToolBar->addWidget( _actionSetOpacity->getWidget());
    ui->visToolBar->addAction( _actionSetParallelProjection->qaction());
    ui->visToolBar->addAction( _actionToggleAxes->qaction());
}   // end createViewMenu


// private
void ClinifaceMain::createToolsMenu()
{
    ui->menu_Tools->addAction( _actionDetectFace->qaction());
    ui->menu_Tools->addAction( _actionGetComponent->qaction());
    ui->menu_Tools->addAction( _actionFillHoles->qaction());
    ui->menu_Tools->addAction( _actionSmooth->qaction());
    ui->menu_Tools->addAction( _actionRemesh->qaction());
    ui->menu_Tools->addAction( _actionCrop->qaction());
    ui->menu_Tools->addSeparator();
    ui->menu_Tools->addAction( _actionTransformToStandardPosition->qaction());
    ui->menu_Tools->addAction( _actionAlignLandmarks->qaction());
    // other coregistration
}   // end createToolsMenu


// private
void ClinifaceMain::createHelpMenu()
{
    ui->menu_Help->addAction( connectDialog( ui->action_About, new Cliniface::AboutDialog(this)));
    ui->menu_Help->addAction( connectDialog( ui->action_Help, new Cliniface::HelpDialog(this)));
    //ui->menu_Help->addAction( connectDialog( ui->action_Licensing, new Cliniface::LicensesDialog(this)));
    ui->menu_Help->addSeparator();
    _fam->dialog()->setWindowTitle( APP_NAME + QString(" | Plugins"));
    ui->menu_Help->addAction( connectDialog( ui->action_Plugins, _fam->dialog()));
}   // end createHelpMenu


void ClinifaceMain::createToolBar()
{
    ui->mainToolBar->addAction( _actionLoadFaceModels->qaction());
    ui->mainToolBar->addAction( _actionSaveFaceModels->qaction());
    if ( ActionExportPDF::isAvailable())
        ui->mainToolBar->addAction( _actionExportPDF->qaction());
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( _actionDetectFace->qaction());
    ui->mainToolBar->addAction( _actionGetComponent->qaction());
    ui->mainToolBar->addAction( _actionFillHoles->qaction());
    ui->mainToolBar->addAction( _actionSmooth->qaction());
    ui->mainToolBar->addAction( _actionRemesh->qaction());
    ui->mainToolBar->addAction( _actionCrop->qaction());
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( _actionTransformToStandardPosition->qaction());
    ui->mainToolBar->addAction( _actionAlignLandmarks->qaction());
    ui->mainToolBar->addAction( _actionToggleCameraActorInteraction->qaction());
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( _actionSynchroniseCameraMovement->qaction());
    ui->mainToolBar->addAction( _actionOrientCameraToLeftFace->qaction());
    ui->mainToolBar->addAction( _actionOrientCameraToFrontFace->qaction());
    ui->mainToolBar->addAction( _actionOrientCameraToRightFace->qaction());
    ui->mainToolBar->addAction( _actionResetCamera->qaction());
    ui->mainToolBar->addAction( _actionMarquee->qaction());

    // Append a space and then the logo to the toolbar.
    QWidget* emptySpacer = new QWidget();
    emptySpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->mainToolBar->addWidget(emptySpacer);
    QLabel* label = new QLabel;
    label->setText( QString("<a href=\"") + APP_WEBSITE + QString("\"><img src=\":/logos/TOOLBAR_LOGO\"/></a>"));
    label->setTextFormat(Qt::RichText);
    label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    label->setOpenExternalLinks(true);
    ui->mainToolBar->addWidget(label);
}   // end createToolBar


void ClinifaceMain::createContextMenu()
{
    _cmenu = new FaceTools::Interactor::ContextMenuInteractor;
    _cmenu->setViewer( _mfmv->centreViewer());

    _cmenu->addAction( _actionSetFocus);
    _cmenu->addSeparator();
    _cmenu->addAction( _actionRenameLandmark);
    _cmenu->addAction( _actionAddLandmark);
    _cmenu->addAction( _actionDeleteLandmark);
    _cmenu->addSeparator();
    _cmenu->addAction( _actionRenamePath);
    _cmenu->addAction( _actionAddPath);
    _cmenu->addAction( _actionDeletePath);
}   // end createContextMenu


void ClinifaceMain::createVisualisations()
{
    using namespace FaceTools::Vis;
    _actionEditLandmarks = new ActionEditLandmarks( "Edit Landmarks", QIcon(":/icons/MARKER"), _feei, ui->statusbar);
    _actionEditLandmarks->addProcessOn( ChangeEvent( LOADED_MODEL, true));
    _fam->addAction( _actionEditLandmarks);
    _actionEditPaths = new ActionEditPaths( "Edit Paths", QIcon(":/icons/CALIPERS"), _feei, ui->statusbar);
    _fam->addAction( _actionEditPaths);
    _actionVisOutlines = new ActionVisualise( new OutlinesVisualisation("Outlines", QIcon(":/icons/OUTLINES"), QKeySequence(Qt::Key_O)));
    _fam->addAction( _actionVisOutlines);
    _fam->addAction( new ActionVisualise( new BoundingVisualisation));
}   // end createVisualisations


void ClinifaceMain::createFileIO()
{
    QString pdfLatexFilePath = PDF_LATEX;
#ifdef _WIN32
    pdfLatexFilePath = QDir( QApplication::applicationDirPath()).filePath(PDF_LATEX);
#endif
    const QString idtfConverterFilePath = QDir( QApplication::applicationDirPath()).filePath(IDTF_CONVERTER);
    ActionExportPDF::init( pdfLatexFilePath.toStdString(), idtfConverterFilePath.toStdString());

    // PDF Reporting (sample)
    _actionExportPDF = new ActionExportPDF( new FaceTools::Report::SampleReport("Create Report (Sample)"), QIcon(":/icons/PDF"),
                                            this, ui->progressBar);
    _actionExportPDF->setLogoResource(":/logos/PDF_LOGO");
    _fam->addAction( _actionExportPDF);

    using namespace FaceTools::FileIO;
    FaceModelManager* fmm = _fam->modelManager();
    fmm->add( new FaceModelXMLFileHandler);    // Default (preferred)
    fmm->add( new FaceModelOBJFileHandler);
    fmm->add( new FaceModelU3DFileHandler);
    //FaceModelAssImpFileHandlerFactory::printAvailableFormats( std::cerr);
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ask"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ase"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("fbx"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("dxf"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("q3s"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("q3o"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ac"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("acc"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ac3d"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ifc"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ifczip"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("raw"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("sib"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("nff"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("enff"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("xgl"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("zgl"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ter"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ply"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("mot"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("lws"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("lwo"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("lxo"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("stl"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("blend"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("b3d"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ms3d"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("ndo"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("off"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("vta"));
    fmm->add( FaceModelAssImpFileHandlerFactory::make("smd"));

    _actionLoadFaceModels = new ActionLoadFaceModels( "&Open", QIcon(":/icons/LOAD"), QKeySequence(Qt::CTRL + Qt::Key_O), fmm->loader());
    _fam->addAction(_actionLoadFaceModels);
    _actionLoadDirFaceModels = new ActionLoadDirFaceModels( "Open &Dir", QIcon(":/icons/LOAD_DIR"), fmm->loader());
    _fam->addAction(_actionLoadDirFaceModels);
    _actionSaveFaceModels = new ActionSaveFaceModels( "&Save", QIcon(":/icons/SAVE"), QKeySequence(Qt::CTRL + Qt::Key_S), fmm, this);
    _fam->addAction(_actionSaveFaceModels);
    _actionSaveAsFaceModel = new ActionSaveAsFaceModel( "Save &As", QIcon(":/icons/SAVE_AS"), fmm, this);
    _fam->addAction(_actionSaveAsFaceModel);
    _actionCloseFaceModels = new ActionCloseFaceModels( "&Close", QIcon(":/icons/CLOSE"), QKeySequence(Qt::CTRL + Qt::Key_W), fmm, this);
    _fam->addAction(_actionCloseFaceModels);
    _actionCloseAllFaceModels = new ActionCloseAllFaceModels( "Close All", fmm, this);
    _fam->addAction(_actionCloseAllFaceModels);
}   // end createFileIO


void ClinifaceMain::createActions()
{
    _fam->addAction( new ActionMapSurfaceData); // Map Surface Data

    // Adjust camera to look at face
    _actionOrientCameraToFrontFace = new ActionOrientCameraToFace("Orient Camera to Front Face", QIcon(":/icons/ORIENT_CAMERA"));
    _actionOrientCameraToFrontFace->addProcessOn( LOADED_MODEL);
    _fam->addAction( _actionOrientCameraToFrontFace);

    // Adjust camera to look at left face
    _actionOrientCameraToLeftFace = new ActionOrientCameraToFace("Orient Camera to Left Face", QIcon(":/icons/LOOK_RIGHT"));
    _actionOrientCameraToLeftFace->setAngleAboutUpVector( -CV_PI/2);
    _fam->addAction( _actionOrientCameraToLeftFace);

    // Adjust camera to look at right face
    _actionOrientCameraToRightFace = new ActionOrientCameraToFace("Orient Camera to Right Face", QIcon(":/icons/LOOK_LEFT"));
    _actionOrientCameraToRightFace->setAngleAboutUpVector( CV_PI/2);
    _fam->addAction( _actionOrientCameraToRightFace);

    // Transform actor to standard position
    _actionTransformToStandardPosition = new ActionTransformToStandardPosition("Transform to Standard Position", QIcon(":/icons/TRANSFORM"));
    _fam->addAction( _actionTransformToStandardPosition);

    // Get face component
    _actionGetComponent = new ActionGetComponent( "Remove Non-Face Components", QIcon(":/icons/FACE"), ui->progressBar);
    _fam->addAction( _actionGetComponent);

    // Face Detection
    const QString haarModels = QDir( QApplication::applicationDirPath()).filePath( HAAR_CASCADES_MODELS);
    const QString faceShapeLandmarks = QDir( QApplication::applicationDirPath()).filePath( FACE_SHAPE_LANDMARKS);
    _actionDetectFace = new ActionDetectFace( "Detect Face", QIcon(":/icons/DETECT_FACE"),
                                              haarModels, faceShapeLandmarks, this, ui->progressBar);
    _actionDetectFace->execAfter( _actionTransformToStandardPosition);  // Transform to origin.
    _actionDetectFace->execAfter( _actionEditLandmarks);                // Show detected landmarks.
    _actionDetectFace->execAfter( _actionOrientCameraToFrontFace);      // Orient camera to front face.
    _fam->addAction( _actionDetectFace);

    // Reset all cameras
    _actionResetCamera = new ActionResetCamera( "Reset All Cameras", QIcon(":/icons/RESET_CAMERA"));
    _actionResetCamera->addViewer( _mfmv->leftViewer());
    _actionResetCamera->addViewer( _mfmv->centreViewer());
    _actionResetCamera->addViewer( _mfmv->rightViewer());
    _fam->addAction( _actionResetCamera);

    // Orthographic projection
    _actionSetParallelProjection = new ActionSetParallelProjection( "Orthographic Projection", QIcon(":/icons/ORTHOGRAPHIC"));
    _actionSetParallelProjection->addViewer( _mfmv->leftViewer());
    _actionSetParallelProjection->addViewer( _mfmv->centreViewer());
    _actionSetParallelProjection->addViewer( _mfmv->rightViewer());
    _fam->addAction( _actionSetParallelProjection);

    // Save combo screenshot
    _actionSaveScreenshot = new ActionSaveScreenshot( "Save Screenshot (All Viewers)", QIcon(":/icons/SCREENSHOT"));
    _actionSaveScreenshot->addViewer( _mfmv->leftViewer());
    _actionSaveScreenshot->addViewer( _mfmv->centreViewer());
    _actionSaveScreenshot->addViewer( _mfmv->rightViewer());
    _fam->addAction( _actionSaveScreenshot);

    // Smooth
    _actionSmooth = new ActionSmooth( "Smooth Face", QIcon(":/icons/SHAVE"), ui->progressBar);
    _fam->addAction( _actionSmooth);

    // Remesh
    _actionRemesh = new ActionRemesh( "Remesh Face", QIcon(":/icons/REMESH"), ui->progressBar);
    _fam->addAction( _actionRemesh);

    // Toggle Axes
    _actionToggleAxes = new ActionToggleAxes( "World Axes", QIcon(":/icons/AXES"));
    _fam->addAction( _actionToggleAxes);
    _actionToggleAxes->addViewer(_mfmv->leftViewer());
    _actionToggleAxes->addViewer(_mfmv->centreViewer());
    _actionToggleAxes->addViewer(_mfmv->rightViewer());

    // Toggle camera / actor interaction
    _actionToggleCameraActorInteraction = new ActionToggleCameraActorInteraction( "Move Models", QIcon(":/icons/MOVE_MODEL"));
    _fam->addAction( _actionToggleCameraActorInteraction);

    // Toggle synchronisation of camera movement
    _actionSynchroniseCameraMovement = new ActionSynchroniseCameraMovement( "Synchronise Cameras", QIcon(":/icons/SYNCH_CAMERAS"),
                                                                             _actionToggleCameraActorInteraction->interactor());
    _fam->addAction( _actionSynchroniseCameraMovement);
    _actionSynchroniseCameraMovement->addViewer(_mfmv->leftViewer());
    _actionSynchroniseCameraMovement->addViewer(_mfmv->centreViewer());
    _actionSynchroniseCameraMovement->addViewer(_mfmv->rightViewer());

    // Set the camera synchroniser in the camera orienter actions.
    _actionOrientCameraToFrontFace->setCamSynch( _actionSynchroniseCameraMovement);
    _actionOrientCameraToLeftFace->setCamSynch( _actionSynchroniseCameraMovement);
    _actionOrientCameraToRightFace->setCamSynch( _actionSynchroniseCameraMovement);

    // Marquee
    _actionMarquee = new ActionMarquee("Auto-Rotate Views", QIcon(":/icons/MOVIE"), _actionToggleCameraActorInteraction->interactor());
    _fam->addAction( _actionMarquee);
    _actionMarquee->addViewer(_mfmv->leftViewer());
    _actionMarquee->addViewer(_mfmv->centreViewer());
    _actionMarquee->addViewer(_mfmv->rightViewer());

    // Set focus
    _actionSetFocus = new ActionSetFocus( "Set Focus", _feei);
    _fam->addAction(_actionSetFocus);

    // Select a radial area
    _actionRadialSelect = new ActionRadialSelect( "Select Radial Area", QIcon(":/icons/LASSO"), _feei, ui->statusbar);
    _fam->addAction( _actionRadialSelect);

    // Crop
    _actionCrop = new ActionCrop( "Crop Face", QIcon(":/icons/SCALPEL"), ui->progressBar);
    _actionCrop->setSelector( _actionRadialSelect);
    _fam->addAction( _actionCrop);

    // Align Landmarks
    _actionAlignLandmarks = new ActionAlignLandmarks( "Align Landmarks (ICP)", QIcon(":/icons/ALIGN"), ui->progressBar);
    _fam->addAction(_actionAlignLandmarks);

    // Fill holes
    _actionFillHoles = new ActionFillHoles( "Fill Holes", QIcon(":/icons/FILL_HOLES"), ui->progressBar);
    _fam->addAction( _actionFillHoles);

    // Set opacity of view models (and deal with overlaps)
    _actionSetOpacity = new ActionSetOpacity( "Model Opacity", 0.6/*opacity on overlap*/, 0.15/*min opacity*/, this);
    _fam->addAction(_actionSetOpacity);

    // Backface culling
    _actionBackfaceCulling = new ActionBackfaceCulling( "Backface Culling", QIcon(":/icons/OPPOSITE_DIRECTIONS"));
    _fam->addAction(_actionBackfaceCulling);

    // Rename path
    _actionRenamePath = new ActionRenamePath( "Rename Path", QIcon(":/icons/EDIT"), this);
    _actionRenamePath->setEditor( _actionEditPaths);
    _fam->addAction( _actionRenamePath);

    // Add path
    _actionAddPath = new ActionAddPath( "Add Path", QIcon(":/icons/CALIPERS"));
    _actionAddPath->setEditor( _actionEditPaths);
    _fam->addAction( _actionAddPath);

    // Delete path
    _actionDeletePath = new ActionDeletePath( "Delete Path", QIcon(":/icons/ERASER"));
    _actionDeletePath->setEditor( _actionEditPaths);
    _fam->addAction( _actionDeletePath);

    // Rename landmark
    _actionRenameLandmark = new ActionRenameLandmark( "Rename Landmark", QIcon(":/icons/EDIT"), this);
    _actionRenameLandmark->setEditor( _actionEditLandmarks);
    _fam->addAction( _actionRenameLandmark);

    // Add landmark
    _actionAddLandmark = new ActionAddLandmark( "Add Landmark", QIcon(":/icons/MARKER"), this);
    _actionAddLandmark->setEditor( _actionEditLandmarks);
    _fam->addAction( _actionAddLandmark);

    // Delete landmark
    _actionDeleteLandmark = new ActionDeleteLandmark( "Delete Landmark", QIcon(":/icons/ERASER"));
    _actionDeleteLandmark->setEditor( _actionEditLandmarks);
    _fam->addAction( _actionDeleteLandmark);
}   // end createActions



// private
void ClinifaceMain::setupMainViewer()
{
    _mfmv->setLeftResetCameraAction(   _fam->addAction( new ActionResetCamera( "Reset Camera", QIcon(":/icons/RESET_CAMERA"), _mfmv->leftViewer())));
    _mfmv->setCentreResetCameraAction( _fam->addAction( new ActionResetCamera( "Reset Camera", QIcon(":/icons/RESET_CAMERA"), _mfmv->centreViewer())));
    _mfmv->setRightResetCameraAction(  _fam->addAction( new ActionResetCamera( "Reset Camera", QIcon(":/icons/RESET_CAMERA"), _mfmv->rightViewer())));

    _mfmv->setLeftSaveImageAction(   _fam->addAction( new ActionSaveScreenshot( "Save Screenshot", QIcon(":/icons/SCREENSHOT"), _mfmv->leftViewer())));
    _mfmv->setCentreSaveImageAction( _fam->addAction( new ActionSaveScreenshot( "Save Screenshot", QIcon(":/icons/SCREENSHOT"), _mfmv->centreViewer())));
    _mfmv->setRightSaveImageAction(  _fam->addAction( new ActionSaveScreenshot( "Save Screenshot", QIcon(":/icons/SCREENSHOT"), _mfmv->rightViewer())));

    FaceTools::Action::ModelSelector* selector = _fam->selector();

    // Set move actions
    _mfmv->setMoveLeftToCentreAction( _fam->addAction(
                new ActionMoveViewer( _mfmv->centreViewer(), selector, _mfmv->leftViewer(), "Move Right", QIcon(":/icons/MOVE_RIGHT"))));
    _mfmv->setMoveCentreToLeftAction( _fam->addAction(
                new ActionMoveViewer( _mfmv->leftViewer(), selector, _mfmv->centreViewer(), "Move Left", QIcon(":/icons/MOVE_LEFT"))));
    _mfmv->setMoveCentreToRightAction( _fam->addAction(
                new ActionMoveViewer( _mfmv->rightViewer(), selector, _mfmv->centreViewer(), "Move Right", QIcon(":/icons/MOVE_RIGHT"))));
    _mfmv->setMoveRightToCentreAction( _fam->addAction(
                new ActionMoveViewer( _mfmv->centreViewer(), selector, _mfmv->rightViewer(), "Move Left", QIcon(":/icons/MOVE_LEFT"))));

    // Set copy actions
    _mfmv->setCopyLeftToCentreAction( _fam->addAction(
                new ActionCopyViewer( _mfmv->centreViewer(), selector, _mfmv->leftViewer(), "Copy Right", QIcon(":/icons/COPY_RIGHT"))));
    _mfmv->setCopyCentreToLeftAction( _fam->addAction(
                new ActionCopyViewer( _mfmv->leftViewer(), selector, _mfmv->centreViewer(), "Copy Left", QIcon(":/icons/COPY_LEFT"))));
    _mfmv->setCopyCentreToRightAction( _fam->addAction(
                new ActionCopyViewer( _mfmv->rightViewer(), selector, _mfmv->centreViewer(), "Copy Right", QIcon(":/icons/COPY_RIGHT"))));
    _mfmv->setCopyRightToCentreAction( _fam->addAction(
                new ActionCopyViewer( _mfmv->centreViewer(), selector, _mfmv->rightViewer(), "Copy Left", QIcon(":/icons/COPY_LEFT"))));
}   // end setupMainViewer


// public
ClinifaceMain::ClinifaceMain() : QMainWindow(nullptr), ui(new Ui::ClinifaceMain)
{
    ui->setupUi(this);
    setAcceptDrops(true);   // Accept dropping of files onto this widget
    setContextMenuPolicy(Qt::NoContextMenu);

    _mfmv = new FaceTools::MultiFaceModelViewer( this);
    _fam = new FaceTools::Action::FaceActionManager( _mfmv->centreViewer(), 20/*model load limit*/, this);

    // Interaction tracking starts on centre viewer.
    _interactions = new FaceTools::Interactor::ViewerInteractionManager(_mfmv->centreViewer());
    _interactions->addViewer(_mfmv->leftViewer());
    _interactions->addViewer(_mfmv->rightViewer());

    // Needed for some actions that check if models are under the mouse cursor.
    _feei = new FaceTools::Interactor::FaceEntryExitInteractor;
    _feei->setViewer(_mfmv->centreViewer());

    createVisualisations(); // Core visualisations
    createFileIO();         // Core file IO
    createActions();        // Core actions (not visualisations)

    setupMainViewer();

    QWidget* cwidget = new QWidget;
    cwidget->setLayout( new QVBoxLayout);
    cwidget->layout()->setContentsMargins(0,0,0,0);
    cwidget->layout()->addWidget( _mfmv);
    cwidget->layout()->addWidget( ui->progressBar);
    setCentralWidget( cwidget);
    _mfmv->layout()->setContentsMargins(3,1,3,1);

    _fam->loadPlugins();

    createToolBar();
    createFileMenu();
    createViewMenu();
    createToolsMenu();
    createHelpMenu();
    createContextMenu();

    // Locate centrally on desktop
    setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, sizeHint(), qApp->desktop()->availableGeometry()));

    // Update window title whenever an action finishes.
    connect( _fam, &FaceActionManager::onUpdateSelected, this, &ClinifaceMain::doOnUpdateSelected);
    doOnUpdateSelected();
}   // end ctor


// public
ClinifaceMain::~ClinifaceMain()
{
    delete _cmenu;
    delete _feei;
    delete _interactions;
    //delete _fam;
    delete ui;
}   // end dtor


// protected virtual
QSize ClinifaceMain::sizeHint() const { return QSize( 800, 700);}


// public slot
bool ClinifaceMain::loadModel( const QString& fname)
{
    const bool loadedOkay = _actionLoadFaceModels->loadModel( fname);
    if ( !loadedOkay)
        _fam->modelManager()->loader()->showLoadErrors();
    return loadedOkay;
}   // end loadModel


// protected virtual
void ClinifaceMain::dragEnterEvent( QDragEnterEvent *evt)
{
    if ( evt->mimeData()->hasText())
    {
        QString fname = evt->mimeData()->text();
        if ( fname.startsWith("file:///"))
        {
            fname = fname.remove(0, QString("file:///").size()).trimmed();
            if ( _fam->modelManager()->canRead( fname.toStdString()))
                evt->acceptProposedAction();
        }   // end if
    }   // end if
}   // end dragEnterEvent


// protected virtual
void ClinifaceMain::dropEvent( QDropEvent *evt)
{
    QString fname = evt->mimeData()->text();
    fname = fname.remove(0, QString("file:///").size()).trimmed();
    if ( loadModel(fname))
        evt->acceptProposedAction();
}   // end dropEvent


// protected virtual
void ClinifaceMain::closeEvent( QCloseEvent* evt)
{
    if ( _actionCloseAllFaceModels->isEnabled() && !_actionCloseAllFaceModels->process())
        evt->ignore();
}   // end closeEvent


// private slot
void ClinifaceMain::doOnUpdateSelected()
{
    QString wtitle = APP_NAME;
    const FaceTools::FaceControlSet& s = _fam->selector()->selected();
    if ( !s.empty())
    {
        FaceTools::FaceModel* fm = s.first()->data();
        QString smsg = "";
        if ( !fm->isSaved())
            smsg = "(*) ";
        wtitle = smsg + tr(_fam->modelManager()->filepath(fm).c_str());
    }   // end if
    setWindowTitle( wtitle);
}   // end doOnUpdateSelected