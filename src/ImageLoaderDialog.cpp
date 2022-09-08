#include "ImageLoaderDialog.h"
#include "ImageLoaderPlugin.h"
#include "Application.h"

#include <QDebug>

using namespace hdps;

ImageLoaderDialog::ImageLoaderDialog(ImageLoaderPlugin& imageLoaderPlugin) :
    _imageLoaderPlugin(imageLoaderPlugin),
    _scanAction(this, imageLoaderPlugin),
    _imageCollectionsAction(this, imageLoaderPlugin),
    _closeAfterLoadingAction(this, "Close after loading", true, true),
    _loadAction(this, "Load"),
    _cancelAction(this, "Cancel")
{
    setWindowTitle("Load high-dimensional image data");
    setWindowIcon(hdps::Application::getIconFont("FontAwesome").getIcon("images"));

    _closeAfterLoadingAction.setToolTip("Close the dialog when loading is complete");
    _closeAfterLoadingAction.setSettingsPrefix(&imageLoaderPlugin, "CloseAfterLoaded");

    _loadAction.setToolTip("Load the selected image collections");
    _cancelAction.setToolTip("Close the image loader");

    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &ImageLoaderDialog::updateActions);
    connect(&_loadAction, &TriggerAction::triggered, this, &ImageLoaderDialog::loadImageCollections);
    connect(&_cancelAction, &TriggerAction::triggered, this, &ImageLoaderDialog::reject);
    
    connect(&_imageCollectionsAction.getDimensionTagAction(), &DimensionTagAction::beginEstablishDimensionNames, this, [this]() -> void {
        _closeAfterLoadingAction.setEnabled(false);
        _loadAction.setEnabled(false);
        _cancelAction.setEnabled(false);
    });

    connect(&_imageCollectionsAction.getDimensionTagAction(), &DimensionTagAction::endEstablishDimensionNames, this, [this]() -> void {
        _closeAfterLoadingAction.setEnabled(true);
        _loadAction.setEnabled(true);
        _cancelAction.setEnabled(true);
    });

    updateActions();

    auto mainLayout = new QVBoxLayout();

    //mainLayout->setContentsMargins(4, 4, 4, 4);
    //mainLayout->setSpacing(0);
    mainLayout->addWidget(_scanAction.createWidget(this));
    mainLayout->addWidget(_imageCollectionsAction.createWidget(this), 2);

    auto bottomLayout = new QHBoxLayout();

    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->addStretch(1);
    bottomLayout->addWidget(_closeAfterLoadingAction.createWidget(this));
    bottomLayout->addWidget(_loadAction.createWidget(this));
    bottomLayout->addWidget(_cancelAction.createWidget(this));

    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
}

ImageLoaderDialog::~ImageLoaderDialog() = default;

QSize ImageLoaderDialog::sizeHint() const
{
    return QSize(1024, 1024);
}

void ImageLoaderDialog::updateActions()
{
    const auto selectedRows = _imageLoaderPlugin.getImageCollectionsModel().selectionModel().selectedRows();
    const auto hasSelection = !selectedRows.isEmpty();

    _loadAction.setEnabled(hasSelection);

    if (selectedRows.isEmpty()) {
        _loadAction.setText("Load");
        _loadAction.setToolTip("Load image datasets into HDPS");
    }
    else {
        _loadAction.setText(QString("Load %1").arg(QString::number(selectedRows.count())));
        _loadAction.setToolTip(QString("Load %1 image datasets into HDPS").arg(QString::number(selectedRows.count())));
    }
    
    _closeAfterLoadingAction.setEnabled(hasSelection);
}

void ImageLoaderDialog::loadImageCollections()
{
    const auto selectedRows = _imageLoaderPlugin.getSelectedRows();
    const auto hasSelection = !selectedRows.isEmpty();

    for (const auto& selectedImageCollectionIndex : _imageLoaderPlugin.getSelectedRows()) {
        const auto imageCollection = reinterpret_cast<ImageCollection*>(selectedImageCollectionIndex.internalPointer());

        auto currentLevelPoints     = imageCollection->load(&_imageLoaderPlugin);
        auto level0Points = currentLevelPoints;

        const auto level0Size = imageCollection->getTargetSize(Qt::EditRole).toSize();

        auto currentLevelSize = imageCollection->getTargetSize(Qt::EditRole).toSize();

        if (_imageCollectionsAction.getSubsamplingAction().getTypeAction().getCurrentIndex() == static_cast<std::int32_t>(ImageCollection::SubSampling::Type::Pyramid)) {
            auto& levelsAction = _imageCollectionsAction.getSubsamplingAction().getLevelsActions();

            const auto levelFactor = LevelsAction::levelFactors.value(static_cast<LevelsAction::LevelFactor>(levelsAction.getLevelFactorAction().getCurrentIndex()));
            const auto scaleFactor = 1.0f / levelFactor;

            for (int levelIndex = 1; levelIndex <= levelsAction.getNumberOfLevelsAction().getValue(); ++levelIndex) {
                currentLevelSize = currentLevelSize * scaleFactor;

                if (currentLevelSize.width() < 4 || currentLevelSize.height() < 4)
                    break;

                imageCollection->setDatasetName(QString("Level %1").arg(QString::number(levelIndex)));

                imageCollection->setTargetSize(currentLevelSize);

                auto previousLevelPoints = currentLevelPoints;

                currentLevelPoints = imageCollection->load(&_imageLoaderPlugin, currentLevelPoints);
                
                const auto currentLevelFactor = static_cast<std::uint32_t>(powf(levelFactor, levelIndex));

                // Map from current level to level zero
                {
                    SelectionMap selectionMap;

                    for (std::int32_t currentLevelPixelX = 0; currentLevelPixelX < currentLevelSize.width(); ++currentLevelPixelX) {
                        for (std::int32_t currentLevelPixelY = 0; currentLevelPixelY < currentLevelSize.height(); ++currentLevelPixelY) {
                            const auto currentLevelPixelIndex = currentLevelPixelY * currentLevelSize.width() + currentLevelPixelX;

                            std::vector<std::uint32_t> indices;

                            indices.reserve(currentLevelFactor * currentLevelFactor);

                            const auto level0PixelRangeX = QPair<std::uint32_t, std::uint32_t>(currentLevelPixelX * currentLevelFactor, (currentLevelPixelX + 1) * currentLevelFactor);
                            const auto level0PixelRangeY = QPair<std::uint32_t, std::uint32_t>(currentLevelPixelY * currentLevelFactor, (currentLevelPixelY + 1) * currentLevelFactor);

                            for (std::int32_t level0PixelX = level0PixelRangeX.first; level0PixelX < level0PixelRangeX.second; ++level0PixelX) {
                                for (std::int32_t level0PixelY = level0PixelRangeY.first; level0PixelY < level0PixelRangeY.second; ++level0PixelY) {
                                    if (level0PixelX >= level0Size.width() || level0PixelY >= level0Size.height())
                                        break;

                                    indices.push_back(level0PixelY * level0Size.width() + level0PixelX);
                                }
                            }

                            selectionMap[currentLevelPixelIndex] = indices;
                        }
                    }

                    currentLevelPoints->addLinkedData(level0Points, selectionMap);
                }
                
                // Map from level zero to current level
                {
                    SelectionMap selectionMap;

                    for (std::int32_t level0PixelX = 0; level0PixelX < level0Size.width(); ++level0PixelX) {
                        for (std::int32_t level0PixelY = 0; level0PixelY < level0Size.height(); ++level0PixelY) {
                            const auto level0PixelIndex         = level0PixelY * level0Size.width() + level0PixelX;
                            const auto currentLevelPixelX       = static_cast<std::int32_t>(floorf(level0PixelX / static_cast<float>(currentLevelFactor)));
                            const auto currentLevelPixelY       = static_cast<std::int32_t>(floorf(level0PixelY / static_cast<float>(currentLevelFactor)));
                            const auto currentLevelPixelIndex   = currentLevelPixelY * currentLevelSize.width() + currentLevelPixelX;

                            selectionMap[level0PixelIndex] = { static_cast<std::uint32_t>(currentLevelPixelIndex) };
                        }
                    }

                    level0Points->addLinkedData(currentLevelPoints, selectionMap);
                }
            }
        }
    }

    if (_closeAfterLoadingAction.isChecked())
        accept();
}
