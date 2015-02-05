
**inih** is a simple .ini file parser written in C.


## Releases

The releases will be tagged according to [semantic versioning](http://semver.org/spec/v2.0.0.html). All the macros preceded by `// @public` are considered public: they'll only change between major versions. The other macros could change any time. Non-preprocessor identifiers defined in header files are always considered public. New identifiers prefixed with the name of the header file will not warrant a major version bump: e.g., a function `uchar__foo()` may be added to `uchar.h` between minor versions.

Every version tag will be signed by [Malcolm Inglis](http://pool.sks-keyservers.net/pks/lookup?op=vindex&search=0xD020F814) (fingerprint: `0xD020F814`).


## Dependencies

`Package.json` specifies the dependencies of inih: where to get them, and what version to use. [Puck](https://gitorious.org/mcinglis/puck) will parse such a `Package.json`, download the specified repositories, check out the specified version, and, if the dependency has its own `Package.json`, repeat that process for *its* dependencies. With `puck` on your PATH, in the directory of inih:

``` sh
$ puck update
$ puck execute build
```

There's nothing magic to what Puck does, so if you would prefer, you can set up the dependencies manually. You just need to have the dependencies in the `deps` directory within the inih directory, and have them built (if necessary) before building inih.


## License: BSD 3-clause (see LICENSE.txt)

**Copyright (c) 2009 Brush Technology <http://brush.co.nz/>:**

- commit `c407ae1` (initial commit based on version from <https://code.google.com/p/inih/> as of 2015-02-03)

**Copyright (c) 2015 Positronic Manufacturing <http://positronicsolar.com.au/>:**

- all following commits


