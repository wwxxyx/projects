#!/usr/bin/env bash

set -o errexit -o noclobber -o nounset -o pipefail

codec=libmp3lame
extension=mp3
mode=single
container=mp3
jpg=jpg
GREP=$(grep --version | grep -q GNU && echo "grep" || echo "ggrep")

if ! [[ $(type -P "$GREP") ]]; then
    echo "$GREP (GNU grep) is not in your PATH"
    echo "Without it, this script will break."
    echo "On macOS, you may want to try: brew install grep"
    exit 1
fi

if [ "$#" -eq 0 ]; then
        #echo "Usage: bash AXtoMP3.sh [--flac] [--aac] [--single] AUTHCODE {FILES}"
        echo "Usage: bash AXtoMP3.sh [--flac] [--single] AUTHCODE {FILES}"
        exit 1
fi

if [ ! -f .authcode ]; then
    auth_code=$1
    shift
else
    auth_code=`head -1 .authcode`
fi

debug() {
    echo "$(date "+%F %T%z") ${1}"
}

trap 'rm -r -f "${working_directory}"' EXIT
working_directory=`mktemp -d 2>/dev/null || mktemp -d -t 'mytmpdir'`
metadata_file="${working_directory}/metadata.txt"

save_metadata() {
    local media_file
    media_file="$1"
    ffprobe -i "$media_file" 2> "$metadata_file"
}

get_metadata_value() {
    local key
    key="$1"
    normalize_whitespace "$($GREP --max-count=1 --only-matching "${key} *: .*" "$metadata_file" | cut -d : -f 2- | sed -e 's#/##g;s/ (Unabridged)//' | tr -s '[:blank:]' ' ')"
}

get_bitrate() {
    get_metadata_value bitrate | $GREP --only-matching '[0-9]\+'
}

normalize_whitespace() {
    echo $*
}

for path
do
    save_metadata "${path}"
    artist=$(get_metadata_value artist)
    title=$(get_metadata_value album)
    output_directory="$(dirname "${path}")"
    cover_file="${output_directory}/${2}.jpg"
    temp_file_path="${output_directory}/TEMP${2}.mp3"
    full_file_path="${output_directory}/${2}.mp3"
    </dev/null ffmpeg -loglevel panic -y -i "${path}" "$cover_file"
    </dev/null ffmpeg -loglevel panic -stats -activation_bytes "${auth_code}" -i "${path}" -vn -codec:a "${codec}" -ab "$(get_bitrate)k" -map_metadata -1 -metadata title="${title}" -metadata artist="${artist}"  "${temp_file_path}"
    </dev/null ffmpeg -loglevel panic -y -i "${temp_file_path}" -i "${cover_file}" -map 0:0 -map 1:0 -c copy -id3v2_version 3 -metadata:s:v title="Album cover" -metadata:s:v comment="Cover (front)" "${full_file_path}"
    rm -rf "${temp_file_path}"
    rm -rf "${cover_file}"
    rm -rf "${1}"
done
