# stitcher16

Test driver for 16-bit color OpenCV stitching functionality being developed
on behalf of [pathware](https://www.pathware.com/).

Developed by Nick Black (dankamongmen@gmail.com)

## Failure modes
* All-white image: 16-bit unsigned was converted to CV_16S (as done in the
    warp step of detailed16)

## notes (`tiff16`)

* Verifies 16-bit TIFF output:
```
[schwarzgerat](0) $ tiffinfo ~/pathware/stitcher16/16-bit\ Images/Image1.tiff
TIFF Directory at offset 0x57e34f6 (92157174)
  Image Width: 5472 Image Length: 3648
  Bits/Sample: 16
  Compression Scheme: LZW
  Photometric Interpretation: RGB color
  Samples/Pixel: 3
  Rows/Strip: 1
  Planar Configuration: single image plane
  Predictor: horizontal differencing 2 (0x2)
[schwarzgerat](0) $ ./tiff16 ~/pathware/stitcher16/16-bit\ Images/Image1.tiff
Image /home/dank/pathware/stitcher16/16-bit Images/Image1.tiff ([5472 x 3648])
 depth: 2 chan: 3 type: 18
[schwarzgerat](0) $ tiffinfo out16.tiff
TIFF Directory at offset 0x57e34f6 (92157174)
  Image Width: 5472 Image Length: 3648
  Bits/Sample: 16
  Sample Format: unsigned integer
  Compression Scheme: LZW
  Photometric Interpretation: RGB color
  Samples/Pixel: 3
  Rows/Strip: 1
  Planar Configuration: single image plane
  Predictor: horizontal differencing 2 (0x2)
[schwarzgerat](0) $ ls -l ~/pathware/stitcher16/16-bit\ Images/Image1.tiff out16.tiff
-rw-r--r-- 1 dank dank 92186502 2019-09-23 19:55 /home/dank/pathware/stitcher16/16-bit Images/Image1.tiff
-rw-r--r-- 1 dank dank 92186520 2019-10-22 00:22 out16.tiff
[schwarzgerat](0) $
```

* Verifies that we can convert `CV_16UC3` -> `CV_32SC3` -> `CV_16UC3` without
    loss of precision (see `backto16.tiff`).

## notes (`detailed16`)
Defaults:
* `blend`=`multiband`
* `matcher`=`homography`
* `estimator`=`homography`
* `warp`=`spherical`
* `expos_comp`=`gain_blocks`
* `features`=`surf`
* `seam`==`gc_color`

* Using affine for estimator and matcher yielded a null result. Using affine
    for matching only yielded a positive result. Using affine for estimation
    alone yielded a null result. IMHO, the all-homography output was slightly
    better (less banding), at least for 8-bit output.
* Using blend=no yielded an unacceptable result. Blend=feather was similarly
    unaccpetable.
* Must pass cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR to imread() to load
    images as 16-bit ones.
```
Image /home/dank/pathware/stitcher16/DSC068/DSC068_01.tiff ([1280 x 920])
 depth: 2 chan: 3 type: 18
terminate called after throwing an instance of 'cv::Exception'
  what():  OpenCV(4.1.2-dev) /home/dank/src/opencv_contrib/modules/xfeatures2d/src/surf.cpp:894: error: (-215:Assertion failed) !_img.empty() && CV_MAT_DEPTH(imgtype) == CV_8U && (imgcn == 1 || imgcn == 3 || imgcn == 4) in function 'detectAndCompute'
```
  * surf: broken for 16-bit
    * removing assert yields:
```
terminate called after throwing an instance of 'cv::Exception'
  what():  OpenCV(4.1.2-dev) /home/dank/src/opencv/modules/imgproc/src/sumpixels.cpp:580: error: (-210:Unsupported format or combination of formats)  in function 'integral'
```
  * orb: null result
  * sift: broken for 16-bit
    * removing assert yielded many more features detected (thousands rather than
        hundreds), longer running time, positive result, but banding similar to
        8-bit affine match result whether using affine or homography
  * akaze: found insufficient features to merge a full row, though it could
      handle simpler tasks (DSC008, DSC068, etc.). detected far fewer
      features. detection count was not dependent on bit depth (got same
      unusable result with 8-bit loading).
* Second imread() for compositing also needs cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR
```
terminate called after throwing an instance of 'cv::Exception'
  what():  OpenCV(4.1.2-dev) /home/dank/src/opencv/modules/stitching/src/exposure_compensate.cpp:459: error: (-215:Assertion failed) _image.type() == CV_8UC3 in function 'apply'
```
  * Naive removal of assert() led to washed-out, unusable image
* warp type: spherical is default.
  * plane generates garbage.
  * affine generates garbage (tested affine and homography both).

* turning off compensation (--expos_comp no) with affine generates a slightly
  different image in 8-bit mode, a bit blurrier than the compensated
  homographic. uncompensated homographic generates a washed-out image.
  --expos_comp channels didn't seem to affect output.
```
terminate called after throwing an instance of 'cv::Exception'
  what():  OpenCV(4.1.2-dev) /home/dank/src/opencv/modules/stitching/src/blenders.cpp:362: error: (-215:Assertion failed) img.type() == CV_16SC3 || img.type() == CV_8UC3
```
* removed assert
```
    Output [1346 x 920]
 depth: 4 type: 20
terminate called after throwing an instance of 'cv::Exception'
  what():  OpenCV(4.1.2-dev) /home/dank/src/opencv/modules/imgproc/src/pyramids.cpp:1264: error: (-210:Unsupported format or combination of formats)  in function 'pyrDown'
```
* removed assert, added support for CV_U32S in pyrUp and pyrDown
```
Output [1279 x 919]
 depth: 4 type: 20
roi.x: 0 roi.width: 1408 m.cols: 0 roi.y: 320 rou.height: 1408 m.rows: 0
terminate called after throwing an instance of 'cv::Exception'
  what():  OpenCV(4.1.2-dev) /home/dank/src/opencv/modules/core/src/umatrix.cpp:549: error: (-215:Assertion failed) 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols && 0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows in function 'UMat'
via #9  0x00007ffff7e763d6 in cv::detail::MultiBandBlender::feed(cv::_InputArray const&, cv::_InputArray const&, cv::Point_<int>) () at /usr/local/lib/libopencv_stitching.so.4.1
```
* blend no:
```
what():  OpenCV(4.1.2-dev) /home/dank/src/opencv/modules/stitching/src/blenders.cpp:103: error: (-215:Assertion failed) img.type() == CV_16SC3 in function 'feed'
```
* Add CV_32SC3
```
Output [1279 x 919]
 depth: 4 type: 20
==15365== Invalid read of size 1
==15365==    at 0x4870AD2: cv::detail::Blender::feed(cv::_InputArray const&, cv::_InputArray const&, cv::Point_<int>) (in /usr/local/lib/libopencv_stitching.so.4.1.2)
```
* Changed feed() casts to use int from short


## notes (`simple`)

Images as read have depth()==2 (CV_U16), as expected.
Panorama object has depth()==0 (CV_U8), undesirable.

call sequence:
* imgs <- Mat::imread() for each input
* Stitcher::create(SCANS)
* Stitcher::stitch(imgs, cv::Mat pano)
* Mat::imwrite(pano) to output


Stitcher::create(SCANS)
* stitcher->setRegistrationResol(0.6);
* stitcher->setSeamEstimationResol(0.1);
* stitcher->setCompositingResol(ORIG_RESOL);
* stitcher->setPanoConfidenceThresh(1);
* stitcher->setSeamFinder(makePtr<detail::GraphCutSeamFinder>(detail::GraphCutSeamFinderBase::COST_COLOR));
* stitcher->setBlender(makePtr<detail::MultiBandBlender>(false));
* stitcher->setFeaturesFinder(ORB::create());
* stitcher->setInterpolationFlags(INTER_LINEAR);
* SCANS-specific:
  * stitcher->setEstimator(makePtr<detail::AffineBasedEstimator>());
  * stitcher->setWaveCorrection(false);
  * stitcher->setFeaturesMatcher(makePtr<detail::AffineBestOf2NearestMatcher>(false, false));
  * stitcher->setBundleAdjuster(makePtr<detail::BundleAdjusterAffinePartial>());
  * stitcher->setWarper(makePtr<AffineWarper>());
  * stitcher->setExposureCompensator(makePtr<detail::NoExposureCompensator>());
Stitcher::stitch(imgs, pano)
* estimateTransform(imgs), return on != OK
* return composePanorama(pano)
