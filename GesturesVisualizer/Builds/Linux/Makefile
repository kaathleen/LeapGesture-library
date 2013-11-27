# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "LEAP_SCENE_USE_UTIL_GL=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules -I ../../LeapSDK/include -I ../../LeapSDK/util
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib -L"../../LeapSDK/lib/$(shell [[ $$(uname -m) == x86_64 && "$(TARGET_ARCH)" == "-march=native" ]] && echo x64 || echo x86)" -lGL -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt -lGLU -lLeap -Wl,-rpath,\$$ORIGIN/../../../LeapSDK/lib/$(shell [[ $$(uname -m) == x86_64 && "$(TARGET_ARCH)" == "-march=native" ]] && echo x64 || echo x86),-rpath,\$$ORIGIN/../../../LeapSDK/lib/$(shell [[ $$(uname -m) == x86_64 && "$(TARGET_ARCH)" == "-march=native" ]] && echo x64 || echo x86)
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "LEAP_SCENE_USE_UTIL_GL=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules -I ../../LeapSDK/include -I ../../LeapSDK/util
  TARGET := GesturesVisualizer
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "LEAP_SCENE_USE_UTIL_GL=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules -I ../../LeapSDK/include -I ../../LeapSDK/util
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib -L"../../LeapSDK/lib/$(shell [[ $$(uname -m) == x86_64 && "$(TARGET_ARCH)" == "-march=native" ]] && echo x64 || echo x86)" -lGL -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt -lGLU -lLeap -Wl,-rpath,\$$ORIGIN/../../../LeapSDK/lib/$(shell [[ $$(uname -m) == x86_64 && "$(TARGET_ARCH)" == "-march=native" ]] && echo x64 || echo x86),-rpath,\$$ORIGIN/../../../LeapSDK/lib/$(shell [[ $$(uname -m) == x86_64 && "$(TARGET_ARCH)" == "-march=native" ]] && echo x64 || echo x86)
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "LEAP_SCENE_USE_UTIL_GL=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules -I ../../LeapSDK/include -I ../../LeapSDK/util
  TARGET := GesturesVisualizer
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/LeapUtil_9c75cf6f.o \
  $(OBJDIR)/LeapUtilGL_7f869754.o \
  $(OBJDIR)/DefaultWindow_da6e5872.o \
  $(OBJDIR)/GestureFrame_1963002d.o \
  $(OBJDIR)/LMRecorder_53ba0748.o \
  $(OBJDIR)/MainToolbarItemFactory_b8c43e9e.o \
  $(OBJDIR)/MainComponent_a6ffb4a5.o \
  $(OBJDIR)/Main_90ebc5c2.o \
  $(OBJDIR)/juce_core_aff681cc.o \
  $(OBJDIR)/juce_cryptography_25c7e826.o \
  $(OBJDIR)/juce_data_structures_bdd6d488.o \
  $(OBJDIR)/juce_events_79b2840.o \
  $(OBJDIR)/juce_graphics_c8f1e7a4.o \
  $(OBJDIR)/juce_gui_basics_a630dd20.o \
  $(OBJDIR)/juce_gui_extra_7767d6a8.o \
  $(OBJDIR)/juce_opengl_c7e3506c.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking GesturesVisualizer
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning GesturesVisualizer
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)/*
	-@rm -rf $(OBJDIR)

strip:
	@echo Stripping GesturesVisualizer
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/LeapUtil_9c75cf6f.o: ../../LeapSDK/util/LeapUtil.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LeapUtil.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LeapUtilGL_7f869754.o: ../../LeapSDK/util/LeapUtilGL.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LeapUtilGL.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DefaultWindow_da6e5872.o: ../../Source/DefaultWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DefaultWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GestureFrame_1963002d.o: ../../Source/GestureFrame.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GestureFrame.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LMRecorder_53ba0748.o: ../../Source/LMRecorder.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LMRecorder.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainToolbarItemFactory_b8c43e9e.o: ../../Source/MainToolbarItemFactory.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainToolbarItemFactory.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainComponent_a6ffb4a5.o: ../../Source/MainComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_aff681cc.o: ../../JuceLibraryCode/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_25c7e826.o: ../../JuceLibraryCode/modules/juce_cryptography/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_bdd6d488.o: ../../JuceLibraryCode/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_79b2840.o: ../../JuceLibraryCode/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_c8f1e7a4.o: ../../JuceLibraryCode/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_a630dd20.o: ../../JuceLibraryCode/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_7767d6a8.o: ../../JuceLibraryCode/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_c7e3506c.o: ../../JuceLibraryCode/modules/juce_opengl/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
