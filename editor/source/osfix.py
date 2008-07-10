import os.path

def path(relpath):
    return os.path.abspath(relpath).replace('\\', '/')
