EasyCC C++
============

### Requirements
* Boost 1.63.0
```
# Remove old Boost (Optional)
sudo apt-get autoremove libboost1.58-dev

# Start a new installation
cd /tmp
# Download Boost 1.63.0 http://www.boost.org/users/history/version_1_63_0.html
wget https://downloads.sourceforge.net/project/boost/boost/1.63.0/boost_1_63_0.tar.gz?r=&ts=1485798206&use_mirror=pilotfiber
tar xvf boost_1_63_0.tar.gz*
cd boost_1_63_0/
sudo apt-get update
sudo apt-get install build-essential g++ python-dev autotools-dev libicu-dev build-essential libbz2-dev libboost-all-dev

# Build then install
./bootstrap.sh --prefix=/usr/local
./b2
sudo ./b2 install
```
### Project UML
* <a href="https://drive.google.com/file/d/0B8fWEFscW3Z4SzlpMnpETkdPOGs/view">UML Draw.io</a>

### Lexical Analysis
* <a href="https://github.com/amirbawab/EasyCC-CPP/tree/master/lexical">Documentation</a>

### Syntax Analysis
* <a href="https://github.com/amirbawab/EasyCC-CPP/tree/master/syntax">Documentation</a>
