#include "SubsampleImageSettingsWidget.h"

#include "ui_SubsampleImageSettingsWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoaderPlugin.h"

SubsampleImageSettingsWidget::SubsampleImageSettingsWidget(QWidget* parent) :
	_ui{ std::make_unique<Ui::SubsampleImageSettingsWidget>() },
	_subsampleImageSettings(nullptr)
{
	_ui->setupUi(this);
}

SubsampleImageSettingsWidget::~SubsampleImageSettingsWidget()
{
}

void SubsampleImageSettingsWidget::initialize(SubsampleImageSettings* subsampleImageSettings)
{
	_subsampleImageSettings = subsampleImageSettings;

	_ui->subsamplingFilterComboBox->addItems(_subsampleImageSettings->filterNames());

	_ui->subsamplingRatioSpinBox->setValue(_subsampleImageSettings->ratio());
	_ui->subsamplingRatioSlider->setValue(_subsampleImageSettings->ratio());
	_ui->subsamplingFilterComboBox->setCurrentIndex(static_cast<int>(_subsampleImageSettings->filter()));

	connect(_ui->subsamplingRatioSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), _subsampleImageSettings, &SubsampleImageSettings::setRatio);
	connect(_ui->subsamplingFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int currentIndex) { _subsampleImageSettings->setFilter(ImageResamplingFilter(currentIndex)); });
	connect(_subsampleImageSettings, &SubsampleImageSettings::ratioChanged, _ui->subsamplingRatioSpinBox, &QDoubleSpinBox::setValue);
	connect(_subsampleImageSettings, &SubsampleImageSettings::ratioChanged, [=](double ratio) { _ui->subsamplingRatioSlider->setValue(static_cast<int>(ratio)); });
	connect(_ui->subsamplingRatioSlider, &QSlider::valueChanged, [=](int sliderValue) { _subsampleImageSettings->setRatio(static_cast<double>(sliderValue)); });
	connect(_ui->enableSubsamplingCheckbox, &QCheckBox::stateChanged, [=](int state) { _subsampleImageSettings->setEnabled(static_cast<bool>(state)); });

	connect(_subsampleImageSettings, &SubsampleImageSettings::enabledChanged, [=](bool enabled) {
		update();
	});

	update();
}

void SubsampleImageSettingsWidget::update()
{
	const auto enabled = _subsampleImageSettings->enabled();

	_ui->subsamplingRatioLabel->setEnabled(enabled);
	_ui->subsamplingRatioSpinBox->setEnabled(enabled);
	_ui->subsamplingRatioSlider->setEnabled(enabled);
	_ui->subsamplingFilterLabel->setEnabled(enabled);
	_ui->subsamplingFilterComboBox->setEnabled(enabled);
}