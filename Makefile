#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := bt_spp_acceptor_demo

COMPONENT_ADD_INCLUDEDIRS := components/include

include $(IDF_PATH)/make/project.mk

CONFIG_EXAMPLE_WEB_DEPLOY_SF := 1
