//
// RSKInternalUtility.m
//
// Copyright © 2015-present Ruslan Skorb. All rights reserved.
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

#import "RSKInternalUtility.h"

NSString * RSKLocalizedString(NSString *key, NSString *comment)
{
    return [[[RSKInternalUtility class] bundleForStrings] localizedStringForKey:key value:key table:@"RSKImageCropper"];
}

@implementation RSKInternalUtility

+ (NSBundle *)bundleForStrings
{
    static NSBundle *bundle;
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        NSBundle *resourcesBundle;
#ifdef SWIFTPM_MODULE_BUNDLE
        resourcesBundle = SWIFTPM_MODULE_BUNDLE;
#else
        resourcesBundle = [NSBundle bundleForClass:[self class]];
#endif
        NSString *stringsBundlePath = [resourcesBundle pathForResource:@"RSKImageCropperStrings" ofType:@"bundle"];
        bundle = [NSBundle bundleWithPath:stringsBundlePath] ?: resourcesBundle;
    });
    
    return bundle;
}

@end
