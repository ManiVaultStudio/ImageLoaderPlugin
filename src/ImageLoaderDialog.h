#pragma once

#include "ScanAction.h"
#include "ImageCollectionsAction.h"
#include "ImagesAction.h"

#include <actions/ToggleAction.h>
#include <actions/TriggerAction.h>

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
     * @param imageLoaderPlugin Reference to parent image loader plugin
     */
    ImageLoaderDialog(ImageLoaderPlugin& imageLoaderPlugin);

    /** Destructor */
    ~ImageLoaderDialog() override;

    QSize sizeHint() const override;

private:

    void updateActions();

    void loadImageCollections();

private:
    ImageLoaderPlugin&          _imageLoaderPlugin;             /** Reference to parent image loader plugin */
    ScanAction                  _scanAction;                    /** Action for scan settings */
    ImageCollectionsAction      _imageCollectionsAction;        /** Action for interacting with image collections */
    ToggleAction                _closeAfterLoadingAction;       /** Action for loading image collection(s) */
    TriggerAction               _loadAction;                    /** Close the dialog when loading is complete action */
    TriggerAction               _cancelAction;                  /** Close the dialog */
};