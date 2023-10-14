#include "LevelsAction.h"
#include "ImageLoaderPlugin.h"

#include <QGridLayout>
#include <QHBoxLayout>

using namespace mv::gui;

const QMap<LevelsAction::LevelFactor, std::uint32_t> LevelsAction::levelFactors = QMap<LevelsAction::LevelFactor, std::uint32_t>({
    { LevelsAction::LevelFactor::Level2, 2 },
    { LevelsAction::LevelFactor::Level4, 4 },
    { LevelsAction::LevelFactor::Level8, 8 },
    { LevelsAction::LevelFactor::Level16, 16 },
    { LevelsAction::LevelFactor::Level32, 32 },
});

const QMap<LevelsAction::LevelFactor, QString> LevelsAction::levelFactorNames = QMap<LevelsAction::LevelFactor, QString>({
    { LevelsAction::LevelFactor::Level2, "1/2"},
    { LevelsAction::LevelFactor::Level4, "1/4"},
    { LevelsAction::LevelFactor::Level8, "1/8"},
    { LevelsAction::LevelFactor::Level16, "1/16"},
    { LevelsAction::LevelFactor::Level32, "1/32"},
});

LevelsAction::LevelsAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent, "Levels"),
    _imageLoaderPlugin(imageLoaderPlugin),
    _numberOfLevelsAction(this, "Number of levels", 1, 5, 2),
    _levelFactorAction(this, "Level factor", levelFactorNames.values()),
    _infoAction(this, "Info")
{
    setEnabled(false);
    setCheckable(true);

    _numberOfLevelsAction.setToolTip("Number of image pyramid levels");
    _numberOfLevelsAction.setSuffix(" level(s)");
    _numberOfLevelsAction.setDefaultWidgetFlags(IntegralAction::SpinBox);

    _infoAction.setEnabled(false);

    connect(&_numberOfLevelsAction, &IntegralAction::valueChanged, this, [this](const std::int32_t& currentIndex) {
        disconnect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, nullptr);
        {
            for (const auto& selectedRow : _imageLoaderPlugin.getSelectedRows())
                _imageLoaderPlugin.getImageCollectionsModel().setData(selectedRow.siblingAtColumn(ImageCollection::Column::SubsamplingNumberOfLevels), currentIndex);
        }
        connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &LevelsAction::dataChanged);
    });
    
    connect(&_levelFactorAction, &OptionAction::currentIndexChanged, this, [this](const std::int32_t& currentIndex) {
        disconnect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, nullptr);
        {
            for (const auto& selectedRow : _imageLoaderPlugin.getSelectedRows())
                _imageLoaderPlugin.getImageCollectionsModel().setData(selectedRow.siblingAtColumn(ImageCollection::Column::SubsamplingLevelFactor), currentIndex);
        }
        connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &LevelsAction::dataChanged);
    });
    
    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &LevelsAction::dataChanged);
    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &LevelsAction::updateStateFromModel);

    updateStateFromModel();
}

void LevelsAction::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles /*= QList<int>()*/)
{
    if (isColumnInModelIndexRange(topLeft, bottomRight, ImageCollection::Column::SubsamplingType) || isColumnInModelIndexRange(topLeft, bottomRight, ImageCollection::Column::SubsamplingLevelFactor))
        updateStateFromModel();
}

void LevelsAction::updateStateFromModel()
{
    const auto selectedRows         = _imageLoaderPlugin.getSelectedRows();
    const auto numberOfSelectedRows = selectedRows.count();

    if (numberOfSelectedRows == 0) {
        setEnabled(false);

        _numberOfLevelsAction.setValue(2);
        _levelFactorAction.setCurrentIndex(0);
        _infoAction.setString("");
    }

    if (numberOfSelectedRows >= 1) {
        setEnabled(selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingType).data(Qt::EditRole).toInt() == static_cast<std::int32_t>(ImageCollection::SubSampling::Type::Pyramid));
        
        _numberOfLevelsAction.setValue(selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingNumberOfLevels).data(Qt::EditRole).toInt());
        _levelFactorAction.setCurrentIndex(selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingLevelFactor).data(Qt::EditRole).toInt());

        if (numberOfSelectedRows == 1) {
            const auto sourceSize = selectedRows.first().siblingAtColumn(ImageCollection::Column::SourceSize).data(Qt::EditRole).toSize();

            for (int levelIndex = 0; levelIndex < _numberOfLevelsAction.getValue(); ++levelIndex) {
                
            }

            
        }
        else {
            _infoAction.setString("");
        }
    }
}

LevelsAction::Widget::Widget(QWidget* parent, LevelsAction* numberOfLevelsActions, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, numberOfLevelsActions, widgetFlags)
{
    auto layout = new QHBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(numberOfLevelsActions->_numberOfLevelsAction.createWidget(this));
    layout->addWidget(numberOfLevelsActions->_levelFactorAction.createWidget(this));
    layout->addWidget(numberOfLevelsActions->_infoAction.createWidget(this), 1);

    setLayout(layout);
}
