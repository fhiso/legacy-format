SOURCES := serialisation.md dates.md primer.md schemas.md
# Commented out data-model.md because of build errors in IDs
include ../website/run-pandoc.mk

dates.pdf dates.html: DateValue.md DatePeriod.md

DateValue.md: date-pattern.pl
	perl $< --markdown Value > $@

DatePeriod.md: date-pattern.pl
	perl $< --markdown Period > $@
