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

	connect(_colorSettings, &ColorSettings::convertToGrayscaleChanged, [&](const bool& convertToGrayscale)
	{
		_ui->convertToGrayscaleCheckbox->setChecked(convertToGrayscale);
	});

	connect(_ui->convertToGrayscaleCheckbox, &QCheckBox::stateChanged, [&](int state)
	{
		const auto enabled = static_cast<bool>(state);

		_colorSettings->setConvertToGrayscale(enabled);
	});

	_colorSettings->loadSettings();
}