//
// RSKImageCropViewController.h
//
// Copyright © 2014-present Ruslan Skorb. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#import <UIKit/UIKit.h>

NS_HEADER_AUDIT_BEGIN(nullability, sendability)

@protocol RSKImageCropViewControllerDataSource;
@protocol RSKImageCropViewControllerDelegate;

/**
 Types of supported crop modes.
 */
typedef NS_ENUM(NSUInteger, RSKImageCropMode) {
    RSKImageCropModeCircle,
    RSKImageCropModeSquare,
    RSKImageCropModeCustom
};

NS_SWIFT_UI_ACTOR
@interface RSKImageCropViewController : UIViewController

/**
 Designated initializer. Initializes and returns a newly allocated view controller object with the specified image.
 
 @param originalImage The image for cropping.
 */
- (instancetype)initWithImage:(UIImage *)originalImage;

/**
 Initializes and returns a newly allocated view controller object with the specified image and the specified crop mode.
 
 @param originalImage The image for cropping.
 @param cropMode The mode for cropping.
 */
- (instancetype)initWithImage:(UIImage *)originalImage cropMode:(RSKImageCropMode)cropMode;

/**
 Zooms to a specific area of the image so that it is visible.
 
 @param rect A rectangle defining an area of the image. The rectangle should be in the coordinate space of the image crop view controller's view.
 @param animated YES if the scrolling should be animated, NO if it should be immediate.
 */
- (void)zoomToRect:(CGRect)rect animated:(BOOL)animated;

///-----------------------------
/// @name Accessing the Delegate
///-----------------------------

/**
 The receiver's delegate.
 
 @discussion A `RSKImageCropViewControllerDelegate` delegate responds to messages sent by completing / canceling crop the image in the image crop view controller.
 */
@property (weak, nonatomic, nullable) id<RSKImageCropViewControllerDelegate> delegate;

/**
 The receiver's data source.
 
 @discussion A `RSKImageCropViewControllerDataSource` data source provides a custom rect and a custom path for the mask and a custom movement rect for the image.
 */
@property (weak, nonatomic, nullable) id<RSKImageCropViewControllerDataSource> dataSource;

///--------------------------
/// @name Accessing the Image
///--------------------------

/**
 The image for cropping.
 */
@property (strong, nonatomic) UIImage *originalImage;

/// -----------------------------------
/// @name Accessing the Mask Attributes
/// -----------------------------------

/**
 The color of the layer with the mask. Default value is [UIColor colorWithRed:0.0f green:0.0f blue:0.0f alpha:0.7f].
 */
@property (copy, nonatomic) UIColor *maskLayerColor;

/**
 The line width used when stroking the path of the mask layer. Default value is 1.0.
 */
@property (assign, nonatomic) CGFloat maskLayerLineWidth;

/**
 The color to fill the stroked outline of the path of the mask layer, or nil for no stroking. Default valus is nil.
 */
@property (copy, nonatomic, nullable) UIColor *maskLayerStrokeColor;

/**
 The rect of the mask.
 
 @discussion Updating each time before the crop view lays out its subviews.
 */
@property (assign, readonly, nonatomic) CGRect maskRect;

/**
 The path of the mask.
 
 @discussion Updating each time before the crop view lays out its subviews.
 */
@property (copy, readonly, nonatomic) UIBezierPath *maskPath;

/// -----------------------------------
/// @name Accessing the Crop Attributes
/// -----------------------------------

/**
 The mode for cropping. Default value is `RSKImageCropModeCircle`.
 */
@property (assign, nonatomic) RSKImageCropMode cropMode;

/**
 The crop rectangle.
 
 @discussion The value is calculated at run time.
 */
@property (readonly, nonatomic) CGRect cropRect;

/**
 A value that specifies the current rotation angle of the image in radians.
 
@discussion The value is calculated at run time.
 */
@property (readonly, nonatomic) CGFloat rotationAngle;

/**
 A floating-point value that specifies the current scale factor applied to the image.
 
 @discussion The value is calculated at run time.
 */
@property (readonly, nonatomic) CGFloat zoomScale;

/**
 A Boolean value that determines whether the image will always fill the mask space. Default value is `NO`.
 */
@property (assign, nonatomic) BOOL avoidEmptySpaceAroundImage;

/**
 A Boolean value that determines whether the image will always bounce horizontally. Default value is `NO`.
 */
@property (assign, nonatomic) BOOL alwaysBounceHorizontal;

/**
 A Boolean value that determines whether the image will always bounce vertically. Default value is `NO`.
 */
@property (assign, nonatomic) BOOL alwaysBounceVertical;

/**
 A Boolean value that determines whether the mask applies to the image after cropping. Default value is `NO`.
 */
@property (assign, nonatomic) BOOL applyMaskToCroppedImage;

/**
 A Boolean value that controls whether the image bounces past the edge of the movement rect and back again. Default value is `YES`.
 */
@property (assign, nonatomic) BOOL bounces;

/**
 A Boolean value that determines whether the image scaling is animated when the scaling exceeds the maximum or minimum limits. Default value is `YES`.
 */
@property (assign, nonatomic) BOOL bouncesZoom;

/**
 A Boolean value that controls whether the rotaion gesture is enabled. Default value is `NO`.
 
 @discussion To support the rotation when `cropMode` is `RSKImageCropModeCustom` you must implement the data source method `imageCropViewControllerCustomMovementRect:`.
 */
@property (assign, getter=isRotationEnabled, nonatomic) BOOL rotationEnabled;

/// -------------------------------
/// @name Accessing the UI Elements
/// -------------------------------

/**
 The Title Label.
 */
@property (strong, nonatomic, readonly) UILabel *moveAndScaleLabel;

/**
 The Cancel Button.
 */
@property (strong, nonatomic, readonly) UIButton *cancelButton;

/**
 The Choose Button.
 */
@property (strong, nonatomic, readonly) UIButton *chooseButton;

/// -------------------------------------------
/// @name Checking of the Interface Orientation
/// -------------------------------------------

/**
 Returns a Boolean value indicating whether the user interface is currently presented in a portrait orientation.
 
 @return YES if the interface orientation is portrait, otherwise returns NO.
 */
- (BOOL)isPortraitInterfaceOrientation;

/// -------------------------------------
/// @name Accessing the Layout Attributes
/// -------------------------------------

/**
 The inset of the circle mask rect's area within the crop view's area in portrait orientation. Default value is `15.0f`.
 */
@property (assign, nonatomic) CGFloat portraitCircleMaskRectInnerEdgeInset;

/**
 The inset of the square mask rect's area within the crop view's area in portrait orientation. Default value is `20.0f`.
 */
@property (assign, nonatomic) CGFloat portraitSquareMaskRectInnerEdgeInset;

/**
 The vertical space between the top of the 'Move and Scale' label and the top of the crop view's safe area in portrait orientation. Default value is `44.0f`.
 */
@property (assign, nonatomic) CGFloat portraitMoveAndScaleLabelTopAndCropViewSafeAreaTopVerticalSpace;

/**
 The vertical space between the bottom of the crop view's safe area and the bottom of the 'Cancel' button in portrait orientation. Default value is `21.0f`.
 */
@property (assign, nonatomic) CGFloat portraitCropViewSafeAreaBottomAndCancelButtonBottomVerticalSpace;

/**
 The vertical space between the bottom of the crop view's safe area and the bottom of the 'Choose' button in portrait orientation. Default value is `21.0f`.
 */
@property (assign, nonatomic) CGFloat portraitCropViewSafeAreaBottomAndChooseButtonBottomVerticalSpace;

/**
 The horizontal space between the leading of the 'Cancel' button and the leading of the crop view's safe area in portrait orientation. Default value is `13.0f`.
 */
@property (assign, nonatomic) CGFloat portraitCancelButtonLeadingAndCropViewSafeAreaLeadingHorizontalSpace;

/**
 The horizontal space between the trailing of the crop view's safe area and the trailing of the 'Choose' button in portrait orientation. Default value is `13.0f`.
 */
@property (assign, nonatomic) CGFloat portraitCropViewSafeAreaTrailingAndChooseButtonTrailingHorizontalSpace;

/**
 The inset of the circle mask rect's area within the crop view's area in landscape orientation. Default value is `45.0f`.
 */
@property (assign, nonatomic) CGFloat landscapeCircleMaskRectInnerEdgeInset;

/**
 The inset of the square mask rect's area within the crop view's area in landscape orientation. Default value is `45.0f`.
 */
@property (assign, nonatomic) CGFloat landscapeSquareMaskRectInnerEdgeInset;

/**
 The vertical space between the top of the 'Move and Scale' label and the top of the crop view's safe area in landscape orientation. Default value is `12.0f`.
 */
@property (assign, nonatomic) CGFloat landscapeMoveAndScaleLabelTopAndCropViewSafeAreaTopVerticalSpace;

/**
 The vertical space between the bottom of the crop view's safe area and the bottom of the 'Cancel' button in landscape orientation. Default value is `12.0f`.
 */
@property (assign, nonatomic) CGFloat landscapeCropViewSafeAreaBottomAndCancelButtonBottomVerticalSpace;

/**
 The vertical space between the bottom of the crop view's safe area and the bottom of the 'Choose' button in landscape orientation. Default value is `12.0f`.
 */
@property (assign, nonatomic) CGFloat landscapeCropViewSafeAreaBottomAndChooseButtonBottomVerticalSpace;

/**
 The horizontal space between the leading of the 'Cancel' button and the leading of the crop view's safe area in landscape orientation. Default value is `13.0f`.
 */
@property (assign, nonatomic) CGFloat landscapeCancelButtonLeadingAndCropViewSafeAreaLeadingHorizontalSpace;

/**
 The horizontal space between the trailing of the crop view's safe area and the trailing of the 'Choose' button in landscape orientation. Default value is `13.0f`.
 */
@property (assign, nonatomic) CGFloat landscapeCropViewSafeAreaTrailingAndChooseButtonTrailingHorizontalSpace;

@end

/**
 The `RSKImageCropViewControllerDataSource` protocol is adopted by an object that provides a custom rect and a custom path for the mask and a custom movement rect for the image.
 */
NS_SWIFT_UI_ACTOR
@protocol RSKImageCropViewControllerDataSource <NSObject>

/**
 Asks the data source a custom rect for the mask.
 
 @param controller The crop view controller object to whom a rect is provided.
 
 @return A custom rect for the mask.
 */
- (CGRect)imageCropViewControllerCustomMaskRect:(RSKImageCropViewController *)controller;

/**
 Asks the data source a custom path for the mask.
 
 @param controller The crop view controller object to whom a path is provided.
 
 @return A custom path for the mask.
 */
- (UIBezierPath *)imageCropViewControllerCustomMaskPath:(RSKImageCropViewController *)controller;

/**
 Asks the data source a custom rect in which the image can be moved.
 
 @param controller The crop view controller object to whom a rect is provided.
 
 @return A custom rect in which the image can be moved.
 */
- (CGRect)imageCropViewControllerCustomMovementRect:(RSKImageCropViewController *)controller;

@end

/**
 The `RSKImageCropViewControllerDelegate` protocol defines messages sent to a image crop view controller delegate when crop image was canceled or the original image was cropped.
 */
NS_SWIFT_UI_ACTOR
@protocol RSKImageCropViewControllerDelegate <NSObject>

/**
 Tells the delegate that crop image has been canceled.
 */
- (void)imageCropViewControllerDidCancelCrop:(RSKImageCropViewController *)controller;

/**
 Tells the delegate that the original image has been cropped. Additionally provides a crop rect and a rotation angle used to produce image.
 */
- (void)imageCropViewController:(RSKImageCropViewController *)controller didCropImage:(UIImage *)croppedImage usingCropRect:(CGRect)cropRect rotationAngle:(CGFloat)rotationAngle;

@optional

/**
 Tells the delegate that the image has been displayed.
 */
- (void)imageCropViewControllerDidDisplayImage:(RSKImageCropViewController *)controller;

/**
 Tells the delegate that the original image will be cropped.
 */
- (void)imageCropViewController:(RSKImageCropViewController *)controller willCropImage:(UIImage *)originalImage;

@end

NS_HEADER_AUDIT_END(nullability, sendability)
