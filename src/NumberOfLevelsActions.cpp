#include "NumberOfLevelsActions.h"
#include "ImageLoaderPlugin.h"

#include <QGridLayout>
#include <QHBoxLayout>

using namespace hdps::gui;

const QMap<NumberOfLevelsActions::LevelFactor, std::uint32_t> NumberOfLevelsActions::levelFactors = QMap<NumberOfLevelsActions::LevelFactor, std::uint32_t>({
    { NumberOfLevelsActions::Level2, 2 },
    { NumberOfLevelsActions::Level4, 4 },
    { NumberOfLevelsActions::Level8, 8 },
    { NumberOfLevelsActions::Level16, 16 },
});

const QMap<NumberOfLevelsActions::LevelFactor, QString> NumberOfLevelsActions::levelFactorNames = QMap<NumberOfLevelsActions::LevelFactor, QString>({
    { NumberOfLevelsActions::Level2, "1/2"},
    { NumberOfLevelsActions::Level4, "1/4"},
    { NumberOfLevelsActions::Level8, "1/8"},
    { NumberOfLevelsActions::Level16, "1/16"},
});

NumberOfLevelsActions::NumberOfLevelsActions(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin),
    _numberOfLevelsAction(this, "Number of levels", 1, 5, 2, 2),
    _levelFactorAction(this, "Level factor", levelFactorNames.values()),
    _infoAction(this, "Info")
{
    setEnabled(false);
    setText("Subsampling");
    setCheckable(true);

    _numberOfLevelsAction.setToolTip("Number of image pyramid levels");
    _numberOfLevelsAction.setSuffix(" levels");
    _numberOfLevelsAction.setDefaultWidgetFlags(IntegralAction::SpinBox);

    _infoAction.setEnabled(false);

    connect(&_numberOfLevelsAction, &IntegralAction::valueChanged, this, [this](const std::int32_t& currentIndex) {
        for (const auto& selectedRow : _imageLoaderPlugin.getSelectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedRow.siblingAtColumn(ImageCollection::Column::SubsamplingNumberOfLevels), currentIndex);
    });

    connect(&_levelFactorAction, &OptionAction::currentIndexChanged, this, [this](const std::int32_t& currentIndex) {
        for (const auto& selectedRow : _imageLoaderPlugin.getSelectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedRow.siblingAtColumn(ImageCollection::Column::SubsamplingLevelFactor), levelFactors.value(static_cast<NumberOfLevelsActions::LevelFactor>(currentIndex)));
    });

    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &NumberOfLevelsActions::updateStateFromModel);
    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &NumberOfLevelsActions::updateStateFromModel);

    updateStateFromModel();
}

void NumberOfLevelsActions::updateStateFromModel()
{
    const auto selectedRows         = _imageLoaderPlugin.getSelectedRows();
    const auto numberOfSelectedRows = selectedRows.count();

    if (numberOfSelectedRows == 0) {
        setEnabled(false);

        _numberOfLevelsAction.setValue(2);
        _levelFactorAction.setCurrentIndex(levelFactors.key(static_cast<std::int32_t>(LevelFactor::Level2)));
    }

    if (numberOfSelectedRows >= 1) {
        setEnabled(selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingType).data(Qt::EditRole).toInt() == static_cast<std::int32_t>(ImageCollection::SubSampling::Type::Pyramid));
        
        _numberOfLevelsAction.setValue(selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingNumberOfLevels).data(Qt::EditRole).toInt());
        _levelFactorAction.setCurrentIndex(levelFactors.key(selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingLevelFactor).data(Qt::EditRole).toInt()));
    }
}

NumberOfLevelsActions::Widget::Widget(QWidget* parent, NumberOfLevelsActions* numberOfLevelsActions, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, numberOfLevelsActions, widgetFlags)
{
    auto layout = new QHBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(numberOfLevelsActions->_numberOfLevelsAction.createWidget(this));
    layout->addWidget(numberOfLevelsActions->_levelFactorAction.createWidget(this));
    layout->addWidget(numberOfLevelsActions->_infoAction.createWidget(this), 1);

    setLayout(layout);
}
