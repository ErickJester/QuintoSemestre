class AFD:
    def __init__(self):
        self.state = 'q0'
        self.final_states = {'C0', 'C1', 'C2', 'C3', 'C4', 'C5', 'C6', 'C7', 'C8', 'C9'}
        self.transitions = {
            # Transiciones del estado q0
            ('q0', ' '): 'q0',
            ('q0', ';'): 'q0',
            ('q0', '.'): 'P',
            ('q0', '+'): 'S',
            ('q0', '-'): 'S',
            ('q0', '0'): 'C0',
            ('q0', '1'): 'C0',
            ('q0', '2'): 'C0',
            ('q0', '3'): 'C0',
            ('q0', '4'): 'C0',
            ('q0', '5'): 'C0',
            ('q0', '6'): 'C0',
            ('q0', '7'): 'C0',
            ('q0', '8'): 'C0',
            ('q0', '9'): 'C0',
            ('q0', 'i'): 'R_I',
            ('q0', 'a'): 'C5',
            ('q0', 'b'): 'C5',
            ('q0', 'c'): 'R_C',
            ('q0', 'd'): 'C5',
            ('q0', 'e'): 'C5',
            ('q0', 'f'): 'C5',
            ('q0', 'g'): 'C5',
            ('q0', 'h'): 'C5',
            ('q0', 'j'): 'C5',
            ('q0', 'k'): 'C5',
            ('q0', 'l'): 'C5',
            ('q0', 'm'): 'R_M',
            ('q0', 'n'): 'C5',
            ('q0', 'o'): 'C5',
            ('q0', 'p'): 'R_P',
            ('q0', 'q'): 'C5',
            ('q0', 'r'): 'C5',
            ('q0', 's'): 'C5',
            ('q0', 't'): 'C5',
            ('q0', 'u'): 'C5',
            ('q0', 'v'): 'R_V',
            ('q0', 'w'): 'C5',
            ('q0', 'x'): 'C5',
            ('q0', 'y'): 'C5',
            ('q0', 'z'): 'C5',
            ('q0', '/'): 'C6',
            ('q0', '<'): 'C8',
            ('q0', '>'): 'C8',
            ('q0', '='): 'C8',
            ('q0', '!'): 'C8',
            ('q0', '*'): 'C9',
            ('q0', '/'): 'C9',
            ('q0', '+'): 'C9',
            ('q0', '-'): 'C9',
            ('q0', '%'): 'C9',
            # Transiciones para reconocer palabras reservadas
            ('R_I', 'n'): 'R_I',
            ('R_I', 't'): 'C7',
            ('R_I', 'cualquier otro'): 'C5',
            ('R_C', 'l'): 'R_C',
            ('R_C', 'a'): 'R_C',
            ('R_C', 's'): 'R_C',
            ('R_C', 's'): 'C7',
            ('R_C', 'cualquier otro'): 'C5',
            ('R_M', 'a'): 'R_M',
            ('R_M', 'i'): 'R_M',
            ('R_M', 'n'): 'C7',
            ('R_M', 'cualquier otro'): 'C5',
            ('R_P', 'u'): 'R_P',
            ('R_P', 'b'): 'R_P',
            ('R_P', 'l'): 'R_P',
            ('R_P', 'i'): 'R_P',
            ('R_P', 'c'): 'C7',
            ('R_P', 'r'): 'R_P',
            ('R_P', 'i'): 'R_P',
            ('R_P', 'v'): 'R_P',
            ('R_P', 'a'): 'R_P',
            ('R_P', 't'): 'R_P',
            ('R_P', 'e'): 'C7',
            ('R_P', 'cualquier otro'): 'C5',
            ('R_V', 'o'): 'R_V',
            ('R_V', 'i'): 'R_V',
            ('R_V', 'd'): 'C7',
            ('R_V', 'cualquier otro'): 'C5',
            # Otros estados
            ('C5', 'a-z, A-Z, 0-9, _'): 'C5',
            ('C5', '='): 'C8',
            ('C5', '+, -, *, /, %'): 'E',
            ('C5', ';'): 'q0',
            ('C6', '*, /'): 'C6',
            ('C7', 'a-z, A-Z, _'): 'C5',
            ('C7', '0-9'): 'E',
            ('C8', '0-9'): 'C0',
            ('C8', '.'): 'C3',
            ('C8', ';'): 'F',
            ('C8', 'cualquier otro'): 'E',
            ('C9', 'a-z, A-Z, _'): 'C5',
            ('C9', 'class, main, public, private, void, int'): 'E',
            ('C9', ';'): 'F',
            ('C9', 'cualquier otro'): 'E',
        }

    def transition(self, char):
        try:
            if (self.state, char) in self.transitions:
                self.state = self.transitions[(self.state, char)]
            else:
                self.state = 'E'
        except KeyError:
            self.state = 'E'

    def accepts(self):
        return self.state in self.final_states

    def run(self, input_string):
        for char in input_string:
            self.transition(char)
        return self.accepts()

class SemanticAnalyzer:
    def __init__(self):
        self.reserved_words = {'int', 'class', 'main', 'public', 'private', 'void'}
    
    def analyze(self, tokens):
        for token in tokens:
            if token in self.reserved_words:
                if token.isidentifier() and token not in self.reserved_words:
                    return False
        return True

def analyze_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    afd = AFD()
    analyzer = SemanticAnalyzer()

    errors = []
    for line_num, line in enumerate(lines, start=1):
        tokens = line.split()
        if not afd.run(line) or not analyzer.analyze(tokens):
            errors.append(f"Error en línea {line_num}")

    if errors:
        for error in errors:
            print(error)
    else:
        print(f"No hay errores de análisis léxico en el archivo {file_path}.")

if __name__ == "__main__":
    file_path = "EjemploPracticaAnalizador.java"
    analyze_file(file_path)
