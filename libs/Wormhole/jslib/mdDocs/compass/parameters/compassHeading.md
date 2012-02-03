compassHeading
==========

A `CompassHeading` object is returned to the `compassSuccess` callback function when an error occurs.

Properties
----------
- __magneticHeading:__ The heading in degrees from 0 - 359.99 at a single moment in time. _(Number)_
- __trueHeading:__ The heading relative to the geographic North Pole in degrees 0 - 359.99 at a single moment in time. A negative value indicates that the true heading could not be determined.  _(Number)_
- __headingAccuracy:__ The deviation in degrees between the reported heading and the true heading. _(Number)_
- __timestamp:__ The time at which this heading was determined.  _(milliseconds)_

Description
-----------

The `CompassHeading` object is returned to the user through the `compassSuccess` callback function.
