(define ( convertFitsPgm inName outName ) 
	(let* (
		(theImage 
			(car 
				( file-fits-load 
				1 
				inName 
				inName ))) 
		(theOutput 
			(car 
				(file-pgm-save 
					1 
					theImage 
					(car (gimp-image-get-active-drawable theImage))
					outName
					outName
					0)
					)))))


