
PLANTUML= /user/josh/bin/plantuml.jar
DEST_DIR = /user/josh/vmd_uml
SOURCE_DIR= /user/josh/qtworkspace/vmd/lib/dvd

SOURCES += \
	$(shell echo $(SOURCE_DIR)/*.cpp) \
	$(shell echo $(SOURCE_DIR)/*.uml)

# Create the output files as %.png
OUTPUT := $(foreach source,$(SOURCES),$(notdir $(basename $(source))).png)

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT)

%.png : $(SOURCE_DIR)/%.cpp 
	java -jar $(PLANTUML) -o $(DEST_DIR) $<

%.png : $(SOURCE_DIR)/%.uml $(shell echo $(SOURCE_DIR)/*.cpp)
	java -jar $(PLANTUML) -o $(DEST_DIR) $<
