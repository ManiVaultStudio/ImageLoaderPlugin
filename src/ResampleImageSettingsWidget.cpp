#include "ResampleImageSettingsWidget.h"

#include "ui_ResampleImageSettingsWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoaderPlugin.h"

ResampleImageSettingsWidget::ResampleImageSettingsWidget(QWidget* parent) :
	_imageLoaderPlugin(nullptr),
	_ui{ std::make_unique<Ui::ResampleImageSettingsWidget>() }
{
	_ui->setupUi(this);
	
}

ResampleImageSettingsWidget::~ResampleImageSettingsWidget()
{
}

void ResampleImageSettingsWidget::initialize(ImageLoaderPlugin * imageLoaderPlugin)
{
	/*
	_imageLoaderPlugin = imageLoaderPlugin;

	_ui->resamplingFilterComboBox->addItems(_settings.filterNames());

	_ui->resamplingRatioSpinBox->setValue(_settings.ratio());
	_ui->resamplingFilterComboBox->setCurrentIndex(static_cast<int>(_settings.filter()));

	connect(_ui->resamplingRatioSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), &_settings, &ResampleImageSettings::setRatio);
	connect(_ui->resamplingFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int currentIndex) { _settings.setFilter(ImageResamplingFilter(currentIndex)); });
	*/
}