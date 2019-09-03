#include "ResampleImageSettingsWidget.h"

#include "ui_ResampleImageSettingsWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoaderPlugin.h"

ResampleImageSettingsWidget::ResampleImageSettingsWidget(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_ui{ std::make_unique<Ui::ResampleImageSettingsWidget>() }
{
	_ui->setupUi(this);

	_ui->resamplingFilterComboBox->addItems(resampleImageSettings().filterNames());

	_ui->resamplingRatioSpinBox->setValue(resampleImageSettings().ratio());
	_ui->resamplingFilterComboBox->setCurrentIndex(static_cast<int>(resampleImageSettings().filter()));

	connect(_ui->resamplingRatioSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), &resampleImageSettings(), &ResampleImageSettings::setRatio);
	connect(_ui->resamplingFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int currentIndex) { resampleImageSettings().setFilter(ImageResamplingFilter(currentIndex)); });
}

ResampleImageSettingsWidget::~ResampleImageSettingsWidget()
{
}

ResampleImageSettings& ResampleImageSettingsWidget::resampleImageSettings()
{
	return _imageLoaderPlugin->resampleImageSettings();
}
