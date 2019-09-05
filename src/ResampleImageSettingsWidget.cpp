#include "ResampleImageSettingsWidget.h"

#include "ui_ResampleImageSettingsWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoaderPlugin.h"

ResampleImageSettingsWidget::ResampleImageSettingsWidget(QWidget* parent) :
	_ui{ std::make_unique<Ui::ResampleImageSettingsWidget>() },
	_resampleImageSettings(nullptr)
{
	_ui->setupUi(this);
}

ResampleImageSettingsWidget::~ResampleImageSettingsWidget()
{
}

void ResampleImageSettingsWidget::initialize(ResampleImageSettings* resampleImageSettings)
{
	_resampleImageSettings = resampleImageSettings;

	_ui->resamplingFilterComboBox->addItems(_resampleImageSettings->filterNames());

	_ui->resamplingRatioSpinBox->setValue(_resampleImageSettings->ratio());
	_ui->resamplingFilterComboBox->setCurrentIndex(static_cast<int>(_resampleImageSettings->filter()));

	connect(_ui->resamplingRatioSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), _resampleImageSettings, &ResampleImageSettings::setRatio);
	connect(_ui->resamplingFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int currentIndex) { _resampleImageSettings->setFilter(ImageResamplingFilter(currentIndex)); });
}