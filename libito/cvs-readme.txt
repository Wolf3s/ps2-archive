
In order to keep things organized a little, there are a few things those who contribute should do
besides from changing/adding code, so others also have a clue about what has been changed.
So Ive listed a small checklist to follow.

- Shortly document your changes in cvs-changelog.txt
- If you have added features, please document these in the headers in doxygen fashion.
- If you have done one of the tasks listed in cvs-todo.tx, please update this file.
- Add yourself to release/header, if you have not done so already.

In cvs-todo.txt I will add features which will be added to libito in the feature, if anyone
wants to complete one of these tasks, please feel free to do so.

The difference between cvs-changelog.txt and docs/changelog.txt is that cvs-changelog.txt is
for contributers in cvs and docs/changelog.txt is for the users, so only main changes should
be listed in docs/changelog.txt. For instance it wouldn't be necessary to write about something
which doesn't affect the end user in docs/changelog.txt, like "Switched from x to y, works the same,
just a little better", this would be only contributer related.

Unlike other projects, libito binary and source version have the exact same directory structure,
so there is only one enviroment variable LIBITO. 

In order make a release use the make target bin-release or src-release respectively.

I will personally build all releases and post them on my website, so the problem of several
variants of the same version does not arise.

Please use the lastest version of doxygen, the 1.2.x version which comes with cygwin is outdated.

Written by Lukasz Bruun
