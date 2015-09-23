(define ( convertPpmPng inName outName ) 
	(let* (
		(theImage 
			(car 
				( gimp-file-load 
				1 
				inName 
				inName ))) 
		(theOutput 
			(car 
				(file-png-save-defaults 
					1 
					theImage 
					(car (gimp-image-get-active-drawable theImage))
					outName
					outName)
					)))))


