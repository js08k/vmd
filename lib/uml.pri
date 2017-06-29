isEmpty(UML_SOURCE) {
    error( UML_SOURCE variable must be defined before including uml.pri )
}

plantuml.input = UML_SOURCE
plantuml.depends = ${QMAKE_IN_FILE}
plantuml.output  = ${QMAKE_FILE_BASE}.png
plantuml.commands = plantuml ${QMAKE_IN_FILE}
plantuml.CONFIG = no_link
QMAKE_EXTRA_COMPILERS += plantuml
