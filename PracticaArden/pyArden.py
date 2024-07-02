import re

def leer_afd(archivo):
    with open(archivo, 'r') as file:
        lineas = file.readlines()
    
    alfabeto = lineas[0].strip().split(', ')
    estados = lineas[1].strip().split(', ')
    estados_aceptacion = lineas[2].strip().split(', ')
    transiciones = {}
    for linea in lineas[3:]:
        estado, trans = linea.strip().split(' = ')
        transiciones[estado] = trans
    
    return alfabeto, estados, estados_aceptacion, transiciones

def simplificar_expresion(expresion, estado, expresiones):
    partes = expresion.split(' + ')
    resultado = []

    for parte in partes:
        if parte == '/':
            resultado.append('')
        elif len(parte) > 1:
            simbolo, siguiente_estado = parte[0], parte[1:]
            if siguiente_estado == estado:
                resultado.append(simbolo + '*')
            elif siguiente_estado in expresiones:
                if expresiones[siguiente_estado]:
                    nueva_parte = simbolo + '(' + expresiones[siguiente_estado] + ')'
                    if nueva_parte not in resultado:
                        resultado.append(nueva_parte)
                else:
                    resultado.append(simbolo)
            else:
                resultado.append(simbolo + siguiente_estado)
        else:
            resultado.append(parte)

    resultado = [r for r in resultado if r]
    return ' + '.join(resultado)

def simplificar_expresion_final(expresion):
    expresion = expresion.replace('/ + ', '').replace(' + /', '').replace('/', '')  # Eliminar el lambda redundante
    expresion = re.sub(r'\b(\w)\1*\b', r'\1*', expresion)  # Simplificar aa* a a*
    expresion = re.sub(r'\b(\w)\b \+ \b\1\b', r'\1', expresion)  # Simplificar a + a a a
    expresion = re.sub(r'\(([^()]+)\)', lambda m: m.group(1) if '+' not in m.group(1) else m.group(0), expresion)  # Eliminar paréntesis innecesarios
    expresion = re.sub(r'\*\*', '*', expresion)  # Eliminar Kleene star redundantes
    return expresion

def resolver_arden(transiciones):
    expresiones = {estado: transiciones[estado] for estado in transiciones}
    cambios = True
    
    while cambios:
        cambios = False
        for estado in expresiones:
            ecuacion = expresiones[estado]
            nueva_expresion = simplificar_expresion(ecuacion, estado, expresiones)
            
            if nueva_expresion != expresiones[estado]:
                expresiones[estado] = nueva_expresion
                cambios = True
                print(f"Actualización en el estado {estado}: {nueva_expresion}")
    
    return expresiones

def simplificar_expresion_final(expresion):
    # Eliminar lambda redundante
    expresion = re.sub(r'(\b/)', '', expresion)
    # Simplificar aa* a a*
    expresion = re.sub(r'\b(\w)\1*\b', r'\1*', expresion)
    # Simplificar a + a a a
    expresion = re.sub(r'\b(\w)\b \+ \b\1\b', r'\1', expresion)
    # Eliminar paréntesis innecesarios
    expresion = re.sub(r'\(([^()]+)\)', lambda m: m.group(1) if '+' not in m.group(1) else m.group(0), expresion)
    # Eliminar Kleene star redundantes
    expresion = re.sub(r'\*\*', '*', expresion)
    return expresion

def main():
    archivo_entrada = 'AFD.txt'
    alfabeto, estados, estados_aceptacion, transiciones = leer_afd(archivo_entrada)
    
    print("Alfabeto:", alfabeto)
    print("Estados:", estados)
    print("Estados de aceptación:", estados_aceptacion)
    print("Transiciones:", transiciones)
    
    expresiones_regulares = resolver_arden(transiciones)
    
    expresiones_regulares = {estado: simplificar_expresion_final(exp) for estado, exp in expresiones_regulares.items()}
    
    print("Expresiones regulares:", expresiones_regulares)
    
    estado_inicial = estados[0]
    expresion_regular_inicial = expresiones_regulares.get(estado_inicial, "")
    print(f"L(AFD) = {expresion_regular_inicial}")

if __name__ == '__main__':
    main()
