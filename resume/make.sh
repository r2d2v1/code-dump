#!/bin/bash
latex vijay_resume.tex
dvipdf vijay_resume.dvi 
rm vijay_resume.log
rm vijay_resume.dvi
