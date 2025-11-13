import os
import sys
import subprocess

# YAPPY

last_commit = subprocess.run(['git', 'rev-parse', 'HEAD'], capture_output=True, text=True).stdout.replace('\n', '')
print(f'Last commit is {last_commit}')

current_tag = subprocess.run(['git', 'describe', '--contains', last_commit], capture_output=True, text=True).stdout.replace('\n', '')
print(f"Current tag is {current_tag if current_tag != '' else None}")

try:
    if current_tag == '':
        print('Commit doesn\'t have a tag')
        last_tag = subprocess.run(['git', 'describe', '--abbrev=0', '--tags'], capture_output=True, text=True).stdout.replace('\n', '')
        print(f'Last tag is {last_tag}')

        if last_tag == '' or 'release' in  last_tag:
            version = [1, 2, 7]
        else:
            if last_tag[0] == 'v':
                last_tag = last_tag.replace('v', '', 1)
            version = [int(i) for i in last_tag.split('.')]

        last_commit_with_last_tag = subprocess.run(['git', 'log', '-1', last_tag], capture_output=True, text=True).stdout.split('\n')[0].split()[1]
        print(f'Last commit of tag {last_tag} is {last_commit_with_last_tag}')
        commits = subprocess.run(['git', 'log', f'{last_commit_with_last_tag}..{last_commit}', '--format=%B'], capture_output=True, text=True).stdout.split('\n\n')[::-1]

        for commit in commits:
            if 'BREAKING CHANGE' in commit or 'feat!' in commit or 'fix!' in commit:
                version[0] += 1
                version[1] = 0
                version[2] = 0
            elif 'feat' in commit:
                version[1] += 1
                version[2] = 0
            elif 'fix' in commit:
                version[2] += 1
        
        version_s = f"{'.'.join([str(i) for i in version])}"
        print(f'New version is {version_s}')
        subprocess.run(['git', 'tag', version_s, last_commit])
        print('Successful tagged last commit')
        subprocess.run(['git', 'push', '--tags'])
    else:
        print('Commit does have a tag')
except ValueError:
    sys.exit(1)