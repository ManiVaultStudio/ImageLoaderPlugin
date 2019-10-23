#include "ColorSettingsWidget.h"

#include "ui_ColorSettingsWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoaderPlugin.h"

ColorSettingsWidget::ColorSettingsWidget(QWidget* parent) :
	_ui{ std::make_unique<Ui::ColorSettingsWidget>() },
	_colorSettings(nullptr)
{
	_ui->setupUi(this);
}

void ColorSettingsWidget::initialize(ColorSettings* colorSettings)
{
	_colorSettings = colorSettings;

	connect(_colorSettings, &ColorSettings::convertToGrayscaleChanged, this, &ColorSettingsWidget::onConvertToGrayscaleChanged);

	_colorSettings->emitAll();
}

void ColorSettingsWidget::onConvertToGrayscaleChanged(const bool& convertToGrayscaleChanged)
{
	_ui->convertToGrayscaleCheckbox->setChecked(convertToGrayscaleChanged);
}