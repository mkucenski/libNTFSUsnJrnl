libUsnJrnl
========
Library for accessing Windows Change (USN) Journal records (\$Extend\$UsnJrnl:$J).

License
-------
Copyright &copy; 2017 Matthew A. Kucenski

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Detailed Description
--------------------

This project was created in order to understand the $UsnJrnl format and start evaluating the forensic usefulness of the data contained within. Initial tests, particularly when viewed in conjunction with file system records, brought forward at least one interesting aspect of this data. File rename records are contained within the $UsnJrnl. When conducting an exam, this information could potentially be *very* useful in tracking the source/changes to files over time. Can't figure out where <badfile.xyz> came from? What if it was downloaded using an application, but later renamed? It may not be immediately obvious this is what happened. Since MFT entry and sequence values are contained in the journal, you should be able to accurately link files and their parent directories together.

Dependencies
------------
* [popt](http://www.freecode.com/projects/popt/)
* [Boost Date-Time](http://www.boost.org)
* [autoconf-archive](https://savannah.gnu.org/projects/autoconf-archive/)
* [My libtimeUtils](https://github.com/mkucenski/libtimeUtils)
* ...

Bugs
-----
* ...

Wishlist
-----
* ...

Notes
-----
```
...
```

References
-----
* [](https://msdn.microsoft.com/en-us/library/aa365722(v=vs.85).aspx)

