pipenv is a packaging tool for python
## Step 1 : install pipenv on your machine
*for debian*
>sudo apt install pipenv
>
*for Fedora*
>sudo dnf install pipenv
>
*for FreeBSD*
>pkg install py36-pipenv
>
## Step 2 : Install packages in Pipfile
>pipenv install

## Step 3 : access the pipenv shell to run your python code
>pipenv shell
>python server.py
*The above command will run  __server.py__ *

## Step 4 : developping with pipenv

### adding a package 
>pipenv install <PACKAGE>

This will install the packge and update the Pipfile and Pipfile.lock
_in the case there is no Pipfile and Pipfile.lock, they will be created_

### removing a package
>pipenv uninstall <PACKAGE>
This will uninstall the package and update the Pipfile

