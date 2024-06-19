import re

def read_afd_file(filename):
    print(f"Leyendo archivo {filename}...")
    with open(filename, 'r') as file:
        lines = file.readlines()
    
    alphabet = lines[0].strip().split(', ')
    states = lines[1].strip().split(', ')
    acceptance_states = lines[2].strip().split(', ')
    equations = [line.strip() for line in lines[3:]]
    
    print("Alfabeto:", alphabet)
    print("Estados:", states)
    print("Estados de aceptación:", acceptance_states)
    print("Ecuaciones:", equations)
    
    return alphabet, states, acceptance_states, equations

def parse_equations(equations):
    parsed_equations = {}
    for equation in equations:
        state, terms = parse_equation(equation)
        parsed_equations[state] = terms
    print("Ecuaciones parseadas:", parsed_equations)
    return parsed_equations

def parse_equation(equation):
    state, expression = equation.split(' = ')
    terms = expression.split(' + ')
    parsed_terms = []
    for term in terms:
        if term == "/":
            parsed_terms.append((term, state))
        else:
            match = re.match(r"(\d*)(\w+)", term)
            if match:
                input_symbol = match.group(1)
                next_state = match.group(2)
                parsed_terms.append((input_symbol, next_state))
    return state, parsed_terms

def resolve_equations(states, parsed_equations):
    regex = {state: "" for state in states}
    
    # Inicialmente, asignar la expresión base
    for state, terms in parsed_equations.items():
        regex[state] = ' + '.join([f"{input_symbol}{next_state}" if input_symbol != "/" else f"({state})*" for input_symbol, next_state in terms])

    print("Expresiones iniciales:", regex)

    # Iterar hasta que todas las referencias de estados sean reemplazadas
    converged = False
    while not converged:
        converged = True
        for state in states:
            new_expr = regex[state]
            for s in states:
                if s in new_expr:
                    new_expr = new_expr.replace(s, f"({regex[s]})")
            new_expr = re.sub(r'\((\w+)\)', r'\1', new_expr)  # Simplificar si es posible
            new_expr = simplify_expression(new_expr)
            if new_expr != regex[state]:
                converged = False
            regex[state] = new_expr
            print(f"Expresión actualizada para el estado {state}: {regex[state]}")
    
    final_regex = regex['X0']
    final_regex = re.sub(r'\(\(\)\)\*', '', final_regex)  # Simplificar si es posible
    
    if not final_regex.endswith("λ"):
        final_regex += "+λ"
        
    return final_regex

def simplify_expression(expression):
    # Simplificar expresión removiendo paréntesis innecesarios
    expression = re.sub(r'\((\w+)\)', r'\1', expression)
    expression = re.sub(r'\(\(\)\)\*', '', expression)
    expression = re.sub(r'\+\)', ')', expression)
    expression = re.sub(r'\(\+', '(', expression)
    expression = expression.replace('((', '(').replace('))', ')')
    return expression

def main():
    filename = 'AFD.txt'
    alphabet, states, acceptance_states, equations = read_afd_file(filename)
    parsed_equations = parse_equations(equations)
    result_regex = resolve_equations(states, parsed_equations)
    print(f"L(AFD) = {result_regex}")

if __name__ == "__main__":
    main()
