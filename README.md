# stitcher16

Test driver for 16-bit color OpenCV stitching functionality being developed
on behalf of [pathware](https://www.pathware.com/).

Developed by Nick Black (dankamongmen@gmail.com)

## notes (detailed)
* Using affine for estimator and matcher yielded a null result. Using affine
    for matching only yielded a positive result. Using affine for estimation
    alone yielded a null result. IMHO, the homographic output was slightly
    better (less banding), at least for 8-bit output.
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

## notes (simple)

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
