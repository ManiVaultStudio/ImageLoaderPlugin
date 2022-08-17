#pragma once

#include "ImageCollectionScanner.h"

#include <QWidget>

namespace Ui {
    class CommonSettingsWidget;
}

class ImageLoaderPlugin;

/**
 * Common settings widget
 *
 * Widget class for common settings
 *
 * @author Thomas Kroes
 */
class CommonSettingsWidget : public QWidget
{
public: // Construction/destruction

    /**
     * Constructor
     * @param parent Parent widget
     */
    CommonSettingsWidget(QWidget* parent);

    /** Destructor */
    ~CommonSettingsWidget();

public: // Initialization

    /**
     * Initializes the widget
     * @param imageLoaderPlugin Pointer to image loader plugin
     */
    void initialize(ImageLoaderPlugin* imageLoaderPlugin);

private:
    ImageLoaderPlugin*                          _imageLoaderPlugin;             /** Image loader plugin instance */
    QSharedPointer<Ui::CommonSettingsWidget>    _ui;                            /** Externally loaded UI */
};