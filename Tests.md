### Things to Tackle
---
- [x] What if there are only args and no name of the file/folder in the end?</br>
Eg: `du -k` note: `du -k .` works. 

- [x] `du -uiuo 787`
- [x] `du -k 7897`
- [x] `du 1` -- if file name is a number.
- [x] What is there is no file above the threshold.
- [ ] The second recursive call includes bytes for all the files from the first. 
> Eg: 30928 ./test/hi
> 18172 ./test/bye
> 49100 ./test
> 24 ./aa/hey
> 49124 ./aa

- [ ] `du test -r`
- [ ] `du . -r`