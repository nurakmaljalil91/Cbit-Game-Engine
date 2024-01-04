import sys
import json
import platform
import dload

commands = ['-h', '-help', 'run', 'install']
commands_arg = ['dev', 'build']
count = 0
check_command_arg = False
given_command = ""
given_arg = ""


def install():
    try:
        with open("package.json", "r") as read_json:
            data = json.load(read_json)

            if platform.system() != 'Windows':
                print(f'Sorry, cbepm is not supported in {platform.system()}')
            else:
                for url in data['vendors']:
                    dload.save_unzip(data['vendors'][url], 'Libraries/', True)
                    print(f'download {url} complete')
    except IOError:
        print('Could not find package.json')


def run_arg(command, arg):
    if command == 'install':
        install()
    if command == 'run' and arg == 'dev':
        print('run development')
    if command == '-h' or command == '-help':
        help_arg()


def help_arg():
    print('1. run       = to run the engine')
    print('2. install   = to install the engine')
    print('3. run dev   = to run in development mode')
    print('4. run build = to run in development mode')


if __name__ == '__main__':
    if len(sys.argv) > 1:
        for i in commands:
            if sys.argv[1] == i:
                given_command = sys.argv[1]
                count = count + 1
        if count == 0:
            print(f'{sys.argv[1]} command not found')
        else:
            check_command_arg = True
            count = 0
    else:
        print('write -h for help')
    if check_command_arg:
        if len(sys.argv) > 2:
            for i in commands_arg:
                if sys.argv[2] == i:
                    given_arg = sys.argv[2]
                    count = count + 1
            if count == 0:
                print(f'{sys.argv[2]} command not found, write -h for help')
            else:
                count = 0
    run_arg(given_command, given_arg)
