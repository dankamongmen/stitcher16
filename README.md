# stitcher16

Test driver for 16-bit color OpenCV stitching functionality being developed
on behalf of [pathware](https://www.pathware.com/).

Developed by Nick Black (dankamongmen@gmail.com)

## notes (detailed)
* Using affine for estimator and matcher yielded a null result. Using affine
    for matching only yielded a positive result. Using affine for estimation
    alone yielded a null result. IMHO, the homographic output was slightly
    better (less banding), at least for 8-bit output.

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
