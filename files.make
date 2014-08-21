# A list of directories with source files.
DIRS += src/\
        src/framework/\
        src/generators/\
        src/modules

# A list of classes that need dictionary generation by root
# Make sure to add the relevant lines to the LibraryLinkDef.h file as well
ROOT_DICT_CLASSES:= \
        wfsWaveform
