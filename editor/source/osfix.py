import os.path

def path(relpath):
    return os.path.normpath(os.path.abspath(relpath))
