isEmpty(UML_SOURCE) {
    error( UML_SOURCE variable must be defined before including uml.pri )
}
isEmpty(UML_FORMAT) {
    UML_FORMAT = PNG
}

contains(UML_FORMAT,EPS) {
    UML_OPTIONS += -teps
    suffix = .eps
}
contains(UML_FORMAT,PNG) {
    suffix = .png
}
contains(UML_FORMAT,SVG) {
    UML_OPTION += -tsvg
    suffix = .svg
}

plantuml.input = UML_SOURCE
plantuml.depends = ${QMAKE_IN_FILE}
plantuml.output  = ${QMAKE_FILE_BASE}$${suffix}
plantuml.commands = java -jar $(PLANTUML) -o $${OUT_PWD} ${QMAKE_FILE_NAME}
plantuml.CONFIG = no_link target_predeps
QMAKE_EXTRA_COMPILERS += plantuml
