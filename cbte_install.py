import json
import platform
import dload

with open("cbtepackage.json", "r") as read_json:
    data = json.load(read_json)

if platform.system() is not 'Windows':
    print(f'Sorry, cbepm is not supported in {platform.system()}')
else:
    for url in data['libraries']:
        dload.save_unzip(data['libraries'][url], 'Libraries/')
        print(f'download {url} complete')
