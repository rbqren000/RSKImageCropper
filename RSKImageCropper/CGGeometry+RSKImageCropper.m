//
// CGGeometry+RSKImageCropper.m
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

#import "CGGeometry+RSKImageCropper.h"

// K is a constant such that the accumulated error of our floating-point computations is definitely bounded by K units in the last place.
#ifdef CGFLOAT_IS_DOUBLE
    static const CGFloat kK = 9;
#else
    static const CGFloat kK = 0;
#endif

const CGPoint RSKPointNull = { INFINITY, INFINITY };

CGPoint RSKRectCenterPoint(CGRect rect)
{
    return CGPointMake(CGRectGetMinX(rect) + CGRectGetWidth(rect) / 2,
                       CGRectGetMinY(rect) + CGRectGetHeight(rect) / 2);
}

CGRect RSKRectNormalize(CGRect rect)
{
    CGPoint origin = rect.origin;
    
    CGFloat x = origin.x;
    CGFloat y = origin.y;
    CGFloat ceilX = ceil(x);
    CGFloat ceilY = ceil(y);
    
    if (fabs(ceilX - x) < pow(10, kK) * RSK_EPSILON * fabs(ceilX + x) || fabs(ceilX - x) < RSK_MIN ||
        fabs(ceilY - y) < pow(10, kK) * RSK_EPSILON * fabs(ceilY + y) || fabs(ceilY - y) < RSK_MIN) {
        
        origin.x = ceilX;
        origin.y = ceilY;
    } else {
        origin.x = floor(x);
        origin.y = floor(y);
    }
    
    CGSize size = rect.size;
    
    CGFloat width = size.width;
    CGFloat height = size.height;
    CGFloat ceilWidth = ceil(width);
    CGFloat ceilHeight = ceil(height);
    
    if (fabs(ceilWidth - width) < pow(10, kK) * RSK_EPSILON * fabs(ceilWidth + width) || fabs(ceilWidth - width) < RSK_MIN ||
        fabs(ceilHeight - height) < pow(10, kK) * RSK_EPSILON * fabs(ceilHeight + height) || fabs(ceilHeight - height) < RSK_MIN) {
        
        size.width = ceilWidth;
        size.height = ceilHeight;
    } else {
        size.width = floor(width);
        size.height = floor(height);
    }
    
    return CGRectMake(origin.x, origin.y, size.width, size.height);
}

CGRect RSKRectScaleAroundPoint(CGRect rect, CGPoint point, CGFloat sx, CGFloat sy)
{
    CGAffineTransform translationTransform, scaleTransform;
    translationTransform = CGAffineTransformMakeTranslation(-point.x, -point.y);
    rect = CGRectApplyAffineTransform(rect, translationTransform);
    scaleTransform = CGAffineTransformMakeScale(sx, sy);
    rect = CGRectApplyAffineTransform(rect, scaleTransform);
    translationTransform = CGAffineTransformMakeTranslation(point.x, point.y);
    rect = CGRectApplyAffineTransform(rect, translationTransform);
    return rect;
}

bool RSKPointIsNull(CGPoint point)
{
    return CGPointEqualToPoint(point, RSKPointNull);
}

CGPoint RSKPointRotateAroundPoint(CGPoint point, CGPoint pivot, CGFloat angle)
{
    CGAffineTransform translationTransform, rotationTransform;
    translationTransform = CGAffineTransformMakeTranslation(-pivot.x, -pivot.y);
    point = CGPointApplyAffineTransform(point, translationTransform);
    rotationTransform = CGAffineTransformMakeRotation(angle);
    point = CGPointApplyAffineTransform(point, rotationTransform);
    translationTransform = CGAffineTransformMakeTranslation(pivot.x, pivot.y);
    point = CGPointApplyAffineTransform(point, translationTransform);
    return point;
}

CGFloat RSKPointDistance(CGPoint p1, CGPoint p2)
{
    CGFloat dx = p1.x - p2.x;
    CGFloat dy = p1.y - p2.y;
    return sqrt(pow(dx, 2) + pow(dy, 2));
}

RSKLineSegment RSKLineSegmentMake(CGPoint start, CGPoint end)
{
    return (RSKLineSegment){ start, end };
}

RSKLineSegment RSKLineSegmentRotateAroundPoint(RSKLineSegment line, CGPoint pivot, CGFloat angle)
{
    return RSKLineSegmentMake(RSKPointRotateAroundPoint(line.start, pivot, angle),
                              RSKPointRotateAroundPoint(line.end, pivot, angle));
}

/*
 Equations of line segments:
 
 pA = ls1.start + uA * (ls1.end - ls1.start)
 pB = ls2.start + uB * (ls2.end - ls2.start)
 
 In the case when `pA` is equal `pB` we have:
 
 x1 + uA * (x2 - x1) = x3 + uB * (x4 - x3)
 y1 + uA * (y2 - y1) = y3 + uB * (y4 - y3)
 
 uA = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3) / (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1)
 uB = (x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3) / (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1)
 
 numeratorA = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)
 denominatorA = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1)
 
 numeratorA = (x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)
 denominatorB = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1)
 
 [1] Denominators are equal.
 [2] If numerators and denominator are zero, then the line segments are coincident. The point of intersection is the midpoint of the line segment.
 
 x = (x1 + x2) * 0.5
 y = (y1 + y2) * 0.5
 
 or
 
 x = (x3 + x4) * 0.5
 y = (y3 + y4) * 0.5
 
 [3] If denominator is zero, then the line segments are parallel. There is no point of intersection.
 [4] If `uA` and `uB` is included into the interval [0, 1], then the line segments intersects in the point (x, y).
 
 x = x1 + uA * (x2 - x1)
 y = y1 + uA * (y2 - y1)
 
 or
 
 x = x3 + uB * (x4 - x3)
 y = y3 + uB * (y4 - y3)
 */
CGPoint RSKLineSegmentIntersection(RSKLineSegment ls1, RSKLineSegment ls2)
{
    CGFloat x1 = ls1.start.x;
    CGFloat y1 = ls1.start.y;
    CGFloat x2 = ls1.end.x;
    CGFloat y2 = ls1.end.y;
    CGFloat x3 = ls2.start.x;
    CGFloat y3 = ls2.start.y;
    CGFloat x4 = ls2.end.x;
    CGFloat y4 = ls2.end.y;

    CGFloat numeratorA = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
    CGFloat numeratorB = (x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3);
    CGFloat denominator = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
    
    // Check the coincidence.
    if (fabs(numeratorA) < RSK_EPSILON && fabs(numeratorB) < RSK_EPSILON && fabs(denominator) < RSK_EPSILON) {
        return CGPointMake((x1 + x2) * 0.5, (y1 + y2) * 0.5);
    }
    
    // Check the parallelism.
    if (fabs(denominator) < RSK_EPSILON) {
        return RSKPointNull;
    }
    
    // Check the intersection.
    CGFloat uA = numeratorA / denominator;
    CGFloat uB = numeratorB / denominator;
    if (uA < 0 || uA > 1 || uB < 0 || uB > 1) {
        return RSKPointNull;
    }
    
    return CGPointMake(x1 + uA * (x2 - x1), y1 + uA * (y2 - y1));
}
