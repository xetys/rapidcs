rapidcs
=======

A C++ Combat Engine for OGame Clones

## Contents

1. Introduction
2. Requirements
3. Installing
4. Class documentation
5. Notes




## 1. Introduction


Welcome to the quick documentation of RapidCS.

RapidCS was developed as an experiment and for teaching myself the languages C/C++


RapidCS is a combat system like the one inside OGame. The battle algorithm was implemented refencing to the official description on OWiki.
The aim of RapidCS is maximum of compability to any games running in PHP. Sp the system finally runs as an PHP extension,
and can be called inside PHP.

For the demonstration of how the system can be installed in a running codebase, this project comes with a module
for the opensource project 2Moons, where it was tested in certain situations.


## 2. Requirements

For building the codebase you need these packages:

* php5-dev
* g++

Please note, that the this combatsystem was only tested on debian 6 Squeeze and Ubuntu10 LTS.
But I am sure, at will work also in other linux distributions.


## 3.1 Installing the extension

Extract the files into any directory. 
Then execute these statements:
```
phpize
./configure --enable-combatsystem
make
sudo make install
```

The last command will automatically install the extension into your modules directory of PHP5. To proof, that the installation 
was successfully, test with:
```
php -d"extension=combatsystem.so" -m
```

You should get a list of enabled extensions, if you find "combatsystem", the build was successfully.

Now you have to change the "php.ini" files of your server, by adding the following line at the bottom of the file:
extension=combatsystem.so

The installtion takes effect, after a restart of your http-service.

##  3.2 Testing the combatsystem


The file kstest.php includes the default procudure of communication to RapidCS. 
If everything was done right, you will see the inner version of the CombatSystem, an created array and the original
JSON output from the system.


## 3.3 Installing the system in a running code base


RapidCS do only calculate the battleresult (no derbis data, units lost etc.), and returns the rounddata. Any other information have to be created by
your game. For exmaple you can try RapidCS on 2Moons.

Get the latest version of 2Moons from http://2moons.cc and copy (overwrite) the files from the directory
2Moons_v1.7.

ATTENTION: No warranty for any diseases caused by RapidCS. If you install RapidCS on a running and allready modified 2Moons,
it is strictly recommend, to modify your game step by step manually.


## 4. Class documentation

see docs/readme.txt for class documenation

## 5. Notes


Currently RapidCS calculates a real simulation. That's why it is not possible to use it for large fleet amounts.
We work on a propability theory solution. Until we have no updated in this direction, we recommend to use this system
for maximum 5-speed ogame like systems!








David Steiman (c) 2013
