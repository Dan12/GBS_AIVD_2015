git status
git add .
git rm 'git ls-files -d'
git status
echo "Enter Commit Message"
read -e COMMESS
git commit -m COMMESS -a
git push -u origin master