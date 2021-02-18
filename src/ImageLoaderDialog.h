#pragma once

#include <QDialog>

class ImageLoaderPlugin;

namespace Ui {
    class ImageLoaderDialog;
}

/**
 * Image loader dialog class
 *
 * Dialog class for loader
 *
  * @author Thomas Kroes
 */
class ImageLoaderDialog : public QDialog
{
public:

    /**
     * Constructor
     * @param parent Parent object
     */
    ImageLoaderDialog(QObject* parent = nullptr);

    /** Destructor */
    ~ImageLoaderDialog() override;

    /**
     * Initializes the dialog
     * @param imageLoaderPlugin Pointer to image loader plugin
     */
    void initialize(ImageLoaderPlugin* imageLoaderPlugin);

private:
    QSharedPointer<Ui::ImageLoaderDialog>   _ui;                    /** Externally loaded UI */
    ImageLoaderPlugin*                      _imageLoaderPlugin;     /** Image loader plugin */
};