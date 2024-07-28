import os
import subprocess
import shutil
import json

# Create vendors directory if it doesn't exist
os.makedirs('vendors', exist_ok=True)

# Create temp directory if it doesn't exist
os.makedirs('temp', exist_ok=True)


def check_if_git_installed():
    try:
        subprocess.run(['git', '--version'], check=True)
        print('Git is installed. continuing...')
    except FileNotFoundError:
        print('Git is not installed. Please install git before running this script.')
        exit(1)


def check_if_mingw_installed():
    try:
        subprocess.run(['mingw32-make', '--version'], check=True)
        print('MinGW is installed. continuing...')
    except FileNotFoundError:
        print('MinGW is not installed. Please install MinGW before running this script.')
        exit(1)


def check_if_cmake_installed():
    try:
        subprocess.run(['cmake', '--version'], check=True)
        print('CMake is installed. continuing...')
    except FileNotFoundError:
        print('CMake is not installed. Please install CMake before running this script.')
        exit(1)


def run_command(command, cwd=None):
    print(f"Running command: {command}")  # Print the command being run
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=cwd, shell=True)
    while True:
        output = process.stdout.readline()
        if output == b'' and process.poll() is not None:
            break
        if output:
            print(output.decode().strip())
    stderr = process.communicate()[1]
    if stderr:
        print(stderr.decode().strip())


def clone_repo(repo_url, name):
    url, branch = repo_url.split('@') if '@' in repo_url else (repo_url, None)
    run_command(f"git clone {url} temp/{name}")
    if branch:
        os.chdir(f'temp/{name}')
        run_command(f"git checkout {branch}")
        os.chdir('../../')  # Change back to the original directory


def build_and_install(name):
    os.chdir(f'temp/{name}')
    cmake_build_command = (
        f"cmake -B_builds -DCMAKE_INSTALL_PREFIX=\"../../vendors/{name}\"  -G \"MinGW Makefiles\" "
        f"-DCMAKE_CXX_STANDARD=17"
    )

    cmake_build_command_for_sdl_image = (
        f"cmake -B_builds "
        f"-DCMAKE_INSTALL_PREFIX=\"../../vendors/{name}\" "
        f"-DSDL2_LIBRARY=\"../../vendors/SDL2/lib\" "
        f"-DSDL2_INCLUDE_DIR=\"../../vendors/SDL2/include/SDL2\" "
        # f"-DSDL2MAIN_LIBRARY=\"../../vendors/SDL2/lib/libSDL2main.a\" "
        f"-G \"MinGW Makefiles\" "
        f"-DCMAKE_CXX_STANDARD=17"
    )

    if (name == 'SDL_image' or name == 'SDL_ttf' or name == 'SDL_mixer'):
        run_command(cmake_build_command_for_sdl_image)
    else:
        run_command(cmake_build_command)
    cmake_install_command = (
        f"cmake --build _builds --target install"
    )
    run_command(cmake_install_command)
    os.chdir('../../')  # Change back to the original directory


def main():
    check_if_git_installed()
    check_if_mingw_installed()
    check_if_cmake_installed()

    # Read the dependencies file from JSON file
    with open('library.json', 'r') as file:
        data = json.load(file)

    dependencies = data['dependencies']

    for name, repo_url in dependencies.items():
        if not os.path.exists(f'temp/{name}'):
            # Clone the repository if it doesn't exist
            clone_repo(repo_url, name)

            # Go to folder and build the project
            build_and_install(name)
        else:
            print(f'{name} already exists. Skipping...')


if __name__ == '__main__':
    main()