import os
import click
import json

additions = """
#define RED {0}
enum project_modes{{BASIC, LOWERCOMP, UPPERCOMP, SKILLS, NOAUTONOMOUS, UNITTEST}};
#define PROJECT_MODE {1}

"""

print("Joe's toolchain for easy stuff")


@click.group()
def cli():
    pass


def build(color, mode, slot):
    """Builds, and uploads the program of the specified type"""

    file_text = open(os.path.join("src", "main.cpp"), 'r').read()
    modes = {'Lower Comp': 'LOWERCOMP', 'Upper Comp': 'UPPERCOMP', 'Basic Comp': 'BASIC', 'No Auto': 'NOAUTONOMOUS',
             'Skills': 'SKILLS'}
    specific_additions = additions.format('true' if color == 'red' else 'false', modes[mode])
    open(os.path.join("src", "main.cpp"), 'w+').write(specific_additions + file_text)

    os.system('prosv5 make')
    os.system('prosv5 upload --slot {} --name "{}"'.format(slot, color.upper() + ' ' + mode))

    open(os.path.join("src", "main.cpp"), 'w+').write(file_text)


@cli.command('fullbuild')
def full_build():
    """Builds, and uploads all of the programs"""
    build('red', 'Lower Comp', '1')
    build('red', 'Upper Comp', '2')
    build('red', 'Basic Comp', '3')

    build('blue', 'Lower Comp', '5')
    build('blue', 'Upper Comp', '6')
    build('blue', 'Basic Comp', '7')

    build('red', 'No Auto', '4')
    build('red', 'Skills', '8')


if __name__ == '__main__':
    cli()
