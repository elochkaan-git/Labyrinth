import os
import sys
import subprocess

def run_git(cmd):
    """Run git commands safely with UTF-8 decoding."""
    result = subprocess.run(
        cmd,
        capture_output=True,
        text=True,
        encoding="utf-8",
        errors="replace"
    )
    return result.stdout.strip(' \n\r\t')

last_commit = run_git(['git', 'rev-parse', 'HEAD'])
print(f'Last commit is {last_commit}')

current_tag = run_git(['git', 'describe', '--contains', last_commit])
print(f"Current tag is {current_tag if current_tag != '' else None}")

try:
    if current_tag == '':
        print("Commit doesn't have a tag")

        last_tag = run_git(['git', 'describe', '--abbrev=0', '--tags'])
        print(f'Last tag is {last_tag if last_tag != "" else None}')

        if last_tag == '' or 'release' in last_tag:
            version = [1, 2, 7]
        else:
            tag_clean = last_tag.lstrip('v')
            version = [int(i) for i in tag_clean.split('.')]

        # Get commit pointed to by last tag
        log_output = run_git(['git', 'rev-list', '-1', last_tag])

        if log_output == '':
            raise RuntimeError(f"Tag {last_tag} has no commit or not fetched")

        last_commit_with_last_tag = log_output
        print(f'Last commit of tag {last_tag} is {last_commit_with_last_tag}')

        commits_raw = run_git([
            'git', 'log', f'{last_commit_with_last_tag}..{last_commit}', '--format=%B'
        ])

        commits = [c for c in commits_raw.split('\n\n') if c.strip()]

        for commit in commits:
            if any(key in commit for key in ('BREAKING CHANGE', 'feat!', 'fix!')):
                version[0] += 1
                version[1] = 0
                version[2] = 0
            elif 'feat' in commit:
                version[1] += 1
                version[2] = 0
            elif 'fix' in commit:
                version[2] += 1

        version_s = '.'.join(map(str, version))
        print(f'New version is {version_s}')

        subprocess.run(['git', 'tag', version_s, last_commit])
        print('Successfully tagged last commit')
        subprocess.run(['git', 'push', '--tags'])

    else:
        print('Commit already has a tag')

except Exception as e:
    print("ERROR:", e)
    sys.exit(1)
