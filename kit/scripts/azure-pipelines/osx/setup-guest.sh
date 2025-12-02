#!/bin/sh
export VM_DIRECTORY=`ls ~/Parallels | grep kmpkg-osx`
export SSH_KEY="$HOME/Parallels/$VM_DIRECTORY/id_guest"
export SSH_PUBLIC_KEY="$SSH_KEY.pub"
ssh-keygen -P '' -f "$SSH_KEY"
echo Type 'kmpkg' and press enter
ssh-copy-id -i "$SSH_PUBLIC_KEY" kmpkg@kmpkgs-Virtual-Machine.local
echo Keys deployed
ssh kmpkg@kmpkgs-Virtual-Machine.local -i "$SSH_KEY" echo hello from \`hostname\`
scp -i "$SSH_KEY" ./clt.dmg kmpkg@kmpkgs-Virtual-Machine.local:/Users/kmpkg/clt.dmg
scp -i "$SSH_KEY" ./setup-box.sh kmpkg@kmpkgs-Virtual-Machine.local:/Users/kmpkg/setup-box.sh
ssh kmpkg@kmpkgs-Virtual-Machine.local -i "$SSH_KEY" chmod +x /Users/kmpkg/setup-box.sh
ssh kmpkg@kmpkgs-Virtual-Machine.local -i "$SSH_KEY" /Users/kmpkg/setup-box.sh
