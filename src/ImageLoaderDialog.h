#pragma once

#include "ScanAction.h"
#include "ImageCollectionsAction.h"
#include "ImagesAction.h"
#include "SubsamplingAction.h"

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

private:
    ImageLoaderPlugin&          _imageLoaderPlugin;             /** Reference to parent image loader plugin */
    ScanAction                  _scanAction;                    /** Action for scan settings */
    ImageCollectionsAction      _imageCollectionsAction;        /** Action for interacting with image collections */
    SubsamplingAction           _subsamplingAction;             /** Action for image subsampling settings */
    ToggleAction                _closeAfterLoadingAction;       /** Action for loading image collection(s) */
    TriggerAction               _loadAction;                    /** Close the dialog when loading is complete action */
};