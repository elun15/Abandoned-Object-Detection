### UPDATE: ###

A new article describing this method in greater detail is now available:

    St-Charles, P.-L., Bilodeau, G.-A., Bergevin, R., "Universal Background Subtraction Using Word
    Consensus Models", IEEE Transactions on Image Processing, Vol. 25, Issue 10, 2016, pp. 4768 - 4781

You can obtain a copy of this article [here](http://www.polymtl.ca/litiv/en/pub/index.php).

-------------------------------------------------------------------------------------------------------

This directory contains a 'cleaned' version of the PAWCS method configuration as presented in
the 2015 WACV paper 'A Self-Adjusting Approach to Change Detection Based on Background Word Consensus'.

The main class used for background subtraction is `BackgroundSubtractorPAWCS`; all other files
contain either dependencies, utilities or interfaces for this method. It is based on OpenCV's
`BackgroundSubtractor` interface, and has been tested with version 3.0.0b. By default,
its constructor uses the parameters suggested in the paper.


TL;DR :

```cpp
BackgroundSubtractorPAWCS bgs(/*...*/);
bgs.initialize(/*...*/);
for(/*all frames in the video*/) {
    //...
    bgs(input,output);
    //...
}
```

**Note**: this repository is kept here as a stand-alone implementation reference; for the latest
version of the algorithm (with better optimization), see [our framework on github](https://github.com/plstcharles/litiv).

See LICENSE.txt for terms of use and contact information.