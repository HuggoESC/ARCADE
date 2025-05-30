# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_x64
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq ($(shell echo "test"), "test")
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

ifeq ($(origin CC), default)
  CC = gcc
endif
ifeq ($(origin CXX), default)
  CXX = g++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
RESCOMP = windres
INCLUDES += -I../external/raylib-master/src -I../external/raylib-master/src/external/glfw/include
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS +=
LDDEPS +=
LINKCMD = $(AR) -rcs "$@" $(OBJECTS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_x64)
TARGETDIR = ../../bin/Debug
TARGET = $(TARGETDIR)/raylib.lib
OBJDIR = obj/x64/Debug/raylib
DEFINES += -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -g
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),debug_x86)
TARGETDIR = ../../bin/Debug
TARGET = $(TARGETDIR)/raylib.lib
OBJDIR = obj/x86/Debug/raylib
DEFINES += -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -g
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32

else ifeq ($(config),debug_arm64)
TARGETDIR = ../../bin/Debug
TARGET = $(TARGETDIR)/raylib.lib
OBJDIR = obj/ARM64/Debug/raylib
DEFINES += -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -g
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),release_x64)
TARGETDIR = ../../bin/Release
TARGET = $(TARGETDIR)/raylib.lib
OBJDIR = obj/x64/Release/raylib
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

else ifeq ($(config),release_x86)
TARGETDIR = ../../bin/Release
TARGET = $(TARGETDIR)/raylib.lib
OBJDIR = obj/x86/Release/raylib
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -O2
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32 -s

else ifeq ($(config),release_arm64)
TARGETDIR = ../../bin/Release
TARGET = $(TARGETDIR)/raylib.lib
OBJDIR = obj/ARM64/Release/raylib
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -O2
ALL_LDFLAGS += $(LDFLAGS) -s

else ifeq ($(config),debug_rgfw_x64)
TARGETDIR = ../../bin/Debug_RGFW
TARGET = $(TARGETDIR)/raylib.lib
OBJDIR = obj/x64/Debug_RGFW/raylib
DEFINES += -DDEBUG -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -g
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),debug_rgfw_x86)
TARGETDIR = ../../bin/Debug_RGFW
TARGET = $(TARGETDIR)/raylib.lib
OBJDIR = obj/x86/Debug_RGFW/raylib
DEFINES += -DDEBUG -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -g
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32

else ifeq ($(config),debug_rgfw_arm64)
TARGETDIR = ../../bin/Debug_RGFW
TARGET = $(TARGETDIR)/raylib.lib
OBJDIR = obj/ARM64/Debug_RGFW/raylib
DEFINES += -DDEBUG -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -g
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),release_rgfw_x64)
TARGETDIR = ../../bin/Release_RGFW
TARGET = $(TARGETDIR)/raylib.lib
OBJDIR = obj/x64/Release_RGFW/raylib
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

else ifeq ($(config),release_rgfw_x86)
TARGETDIR = ../../bin/Release_RGFW
TARGET = $(TARGETDIR)/raylib.lib
OBJDIR = obj/x86/Release_RGFW/raylib
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -O2
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32 -s

else ifeq ($(config),release_rgfw_arm64)
TARGETDIR = ../../bin/Release_RGFW
TARGET = $(TARGETDIR)/raylib.lib
OBJDIR = obj/ARM64/Release_RGFW/raylib
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP_RGFW -DGRAPHICS_API_OPENGL_33
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -O2
ALL_LDFLAGS += $(LDFLAGS) -s

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/raudio.o
GENERATED += $(OBJDIR)/rcore.o
GENERATED += $(OBJDIR)/rglfw.o
GENERATED += $(OBJDIR)/rmodels.o
GENERATED += $(OBJDIR)/rshapes.o
GENERATED += $(OBJDIR)/rtext.o
GENERATED += $(OBJDIR)/rtextures.o
GENERATED += $(OBJDIR)/utils.o
OBJECTS += $(OBJDIR)/raudio.o
OBJECTS += $(OBJDIR)/rcore.o
OBJECTS += $(OBJDIR)/rglfw.o
OBJECTS += $(OBJDIR)/rmodels.o
OBJECTS += $(OBJDIR)/rshapes.o
OBJECTS += $(OBJDIR)/rtext.o
OBJECTS += $(OBJDIR)/rtextures.o
OBJECTS += $(OBJDIR)/utils.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking raylib
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning raylib
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CC) -x c-header $(ALL_CFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/raudio.o: ../external/raylib-master/src/raudio.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rcore.o: ../external/raylib-master/src/rcore.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rglfw.o: ../external/raylib-master/src/rglfw.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rmodels.o: ../external/raylib-master/src/rmodels.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rshapes.o: ../external/raylib-master/src/rshapes.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rtext.o: ../external/raylib-master/src/rtext.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rtextures.o: ../external/raylib-master/src/rtextures.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/utils.o: ../external/raylib-master/src/utils.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif