#NuraniLibrary Data Experiments

##0. UI Ideas
The SWORD project(s) have been coming up with UI to handle this data for several decades, it is worth checking them out.  The [MacSWORD project](http://www.macsword.com/tour/) has a fairly good UI.

See: http://www.eloquent-bible-study.eu/macsword2/screenshots/MS2_ParallelWithCommentariesAndHebrew.png


##1. Data Formats

**Problem:** *Find the most useful data formats for NuraniLibrary to support.*

It seems nearly every textual document on the web is using its own format.

###1.1 Format A
The [SBL Greek New Testament](http://www.sblgnt.com/) uses an unknown format.  Here is what it looks like:

	010101 N- ----NSF- Βίβλος Βίβλος βίβλος βίβλος
	010101 N- ----GSF- γενέσεως γενέσεως γενέσεως γένεσις
	010101 N- ----GSM- Ἰησοῦ Ἰησοῦ Ἰησοῦ Ἰησοῦς
	010101 N- ----GSM- χριστοῦ χριστοῦ Χριστοῦ Χριστός
	010101 N- ----GSM- υἱοῦ υἱοῦ υἱοῦ υἱός
	010101 N- ----GSM- Δαυὶδ Δαυὶδ Δαυίδ Δαυίδ
	010101 N- ----GSM- υἱοῦ υἱοῦ υἱοῦ υἱός
	010101 N- ----GSM- Ἀβραάμ. Ἀβραάμ Ἀβραάμ Ἀβραάμ
	010102 N- ----NSM- Ἀβραὰμ Ἀβραὰμ Ἀβραάμ Ἀβραάμ
	010102 V- 3AAI-S-- ἐγέννησεν ἐγέννησεν ἐγέννησε(ν) γεννάω
	010102 RA ----ASM- τὸν τὸν τόν ὁ
	010102 N- ----ASM- Ἰσαάκ, Ἰσαάκ Ἰσαάκ Ἰσαάκ
	010102 N- ----NSM- Ἰσαὰκ Ἰσαὰκ Ἰσαάκ Ἰσαάκ
	010102 C- -------- δὲ δὲ δέ δέ
	010102 V- 3AAI-S-- ἐγέννησεν ἐγέννησεν ἐγέννησε(ν) γεννάω
	010102 RA ----ASM- τὸν τὸν τόν ὁ
	010102 N- ----ASM- Ἰακώβ, Ἰακώβ Ἰακώβ Ἰακώβ
	010102 N- ----NSM- Ἰακὼβ Ἰακὼβ Ἰακώβ Ἰακώβ
	010102 C- -------- δὲ δὲ δέ δέ
	010102 V- 3AAI-S-- ἐγέννησεν ἐγέννησεν ἐγέννησε(ν) γεννάω
	010102 RA ----ASM- τὸν τὸν τόν ὁ
	010102 N- ----ASM- Ἰούδαν Ἰούδαν Ἰούδαν Ἰούδας

###1.1 Format B
The [Tischendorf 8th GNT with morphology and lemmatization](https://github.com/morphgnt/tischendorf) uses an unknown format, it looks like this:

	1:1 {C} Pau=loj klhto\j a)po/stoloj Xristou= )Ihsou= dia\ qelh/matoj qeou=, kai\ Swsqe/nhj o( a)delfo/j,
	1:2 th=| e)kklhsi/a| tou= qeou= th=| ou)/sh| e)n Kori/nqw|, h(giasme/noij e)n Xristw=| )Ihsou=, klhtoi=j a(gi/oij, su\n  pa=sin toi=s e)pikaloume/noij to\ o)/noma tou= kuri/ou h(mw=n )Ihsou= Xristou= e)n panti\ to/pw|, au)tw=n kai\ h(mw=n:
	1:3 xa/rij u(mi=n kai\ ei)rh/nh a)po\ qeou= patro\j h(mw=n kai\ kuri/ou )Ihsou= Xristou=.
	1:4 {P} Eu)xaristw= tw=| qew=| mou pa/ntote peri\ u(mw=n e)pi\ th=| xa/riti tou= qeou= th=| doqei/sh| u(mi=n e)n Xristw=| )Ihsou=,

also some files look like this:

	1CO 1:1.1 C *PAU=LOS *PAU=LOS N-NSM 3972 *PAU=LOS ! *PAU=LOS
	1CO 1:1.2 . KLHTO\S KLHTO\S A-NSM 2822 KLHTO/S ! KLHTO/S
	1CO 1:1.3 . A)PO/STOLOS A)PO/STOLOS N-NSM 652 A)PO/STOLOS ! A)PO/STOLOS
	1CO 1:1.4 . *XRISTOU= *XRISTOU= N-GSM 5547 *XRISTO/S ! *XRISTO/S


###1.1 Format C (USFM)
The [Open English Bible](http://openenglishbible.org/) uses a format called [USFM](http://paratext.ubs-translations.org/about/usfm).

USFM [looks like this](https://github.com/openenglishbible/Open-English-Bible/blob/master/sources/kent/usfm/01-Genesis.usfm):

	{{\id GEN
	\ide UTF-8
	\h Genesis
	\mt2 The
	\mt2 first book of Moses
	\mt2 called
	\mt Genesis
	 
	\ms The beginning
	 
	\q1 \c 1 \v 1 In the beginning when God created the heavens and the earth, 
	\q2 \v 2 and while the earth was still unformed and chaotic, 
	\q2 with darkness on the surface of the deep, 


###1.1 Format D (OSIS)
The [Westminster Leningrad Codex (Old Testament)](https://github.com/openscriptures/morphhb) uses the [OSIS format](http://www.bibletechnologies.net/osisCore.2.1.1.xsd).

The OSIS format [looks like this](https://github.com/openscriptures/morphhb/blob/master/wlc/1Chr.xml):

	<?xml version="1.0" encoding="utf-8"?>
	<osis xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.bibletechnologies.net/2003/OSIS/namespace http://www.bibletechnologies.net/osisCore.2.1.1.xsd" xmlns="http://www.bibletechnologies.net/2003/OSIS/namespace">
	  <osisText xml:lang="he" osisIDWork="WLC">
	    <header>
	      <revisionDesc resp="dt">
	        <date>2011.01.21</date>
	        <p>Updated to WLC version 4.14.</p>
	      </revisionDesc>
	      <revisionDesc resp="dt">
	        <date>2008.08.08</date>
	        <p>Convert the Westminster Leningrad Codex files from TEI markup to OSIS book files.</p>
	      </revisionDesc>
	      <work osisWork="WLC" xml:lang="he">
	        <title>Westminster Leningrad Codex</title>
	        <contributor role="edt">Daniel Owens</contributor>
	        <contributor role="edt">David Troidl</contributor>
	        <contributor role="edt">Christopher V. Kimball</contributor>
	        <date>2007.9.16</date>
	        <identifier type="URL">http://www.tanach.us/Tanach.xml</identifier>
	        <rights>Public Domain</rights>
	      </work>
	      <workPrefix path="//w/@lemma" osisWork="Strong" />
	    </header>
	    <div type="book" osisID="1Chr">
	      <chapter osisID="1Chr.1">
	        <verse osisID="1Chr.1.1">
	          <w lemma="121">אָדָ֥ם</w>
	          <w lemma="8352">שֵׁ֖ת</w>
	          <w lemma="583">אֱנֽוֹשׁ</w><seg type="x-sof-pasuq">׃</seg>
	        </verse>
	        <verse osisID="1Chr.1.2">
	          <w lemma="7018">קֵינָ֥ן</w>
	          <w lemma="4111">מַהֲלַלְאֵ֖ל</w>
	          <w lemma="3382">יָֽרֶד</w><seg type="x-sof-pasuq">׃</seg>
	        </verse>
	      </chapter>
	      <chapter osisID="1Chr.8">
	        <verse osisID="1Chr.8.1">
	          <w lemma="c/1144">וּ/בִ֨נְיָמִ֔ן</w>
	          <w lemma="3205">הוֹלִ֖יד</w>
	          <w lemma="853">אֶת</w><seg type="x-maqqef">־</seg><w lemma="1106 a">בֶּ֣לַע</w>
	          <w lemma="1060">בְּכֹר֑/וֹ</w>
	          <w lemma="788">אַשְׁבֵּל֙</w>
	          <w lemma="d/8145">הַ/שֵּׁנִ֔י</w>
	          <w lemma="c/315">וְ/אַחְרַ֖ח</w>
	          <w lemma="d/7992">הַ/שְּׁלִישִֽׁי</w><seg type="x-sof-pasuq">׃</seg>
	        </verse>
	      </chapter>
	    </div>
	  </osisText>
	</osis>


###1.1 Format E
[Strongs concordance](https://github.com/morphgnt/strongs-dictionary-xml) uses an apparently totally custom format which looks like this:

	<?xml version="1.0" encoding="utf-8" standalone="yes"?>
	<!DOCTYPE strongsdictionary [
	   <!ELEMENT strongsdictionary (prologue, entries) >
	   <!ELEMENT prologue (#PCDATA) >
	   <!ELEMENT entries (entry)+ >
	   <!ELEMENT entry (#PCDATA|strongs|greek|pronunciation|latin|see|strongsref|strongs_def|strongs_derivation|kjv_def)* >
	   <!ATTLIST entry strongs CDATA #REQUIRED >
	   <!ELEMENT greek EMPTY >
	
	
	   <!-- unicode is real Greek. translit is SBL-style transliteration. -->
	   <!ATTLIST greek BETA CDATA #REQUIRED 
	                   unicode CDATA #REQUIRED 
	                   translit CDATA #REQUIRED >
	   <!ELEMENT latin (#PCDATA) >
	   <!ELEMENT kjv_def (#PCDATA|strongsref)* >
	   <!ELEMENT strongs_def (#PCDATA|greek|latin|strongsref|pronunciation)* >
	   <!ELEMENT strongs_derivation (#PCDATA|greek|latin|strongsref|pronunciation)* >
	   <!ELEMENT pronunciation EMPTY >
	   <!ATTLIST pronunciation strongs CDATA #REQUIRED >
	   <!ELEMENT strongs (#PCDATA) >
	   <!ELEMENT see EMPTY >
	   <!ATTLIST see language CDATA #REQUIRED
	                 strongs CDATA #REQUIRED >
	   <!ELEMENT strongsref EMPTY >
	   <!ATTLIST strongsref language CDATA #REQUIRED
	                 strongs CDATA #REQUIRED >
	 ]>
	
	<strongsdictionary>
	    <prologue>
	        …PROLOGUE TEXT…
	    </prologue>
	    <entries>
	        <entry strongs="00001">
	            <strongs>1</strongs>
	            <greek BETA="*A" unicode="Α" translit="A"/>
	            <pronunciation strongs="al'-fah"/>
	            <strongs_derivation>of Hebrew origin;</strongs_derivation>
	            <strongs_def> the first letter of the alphabet; figuratively, only
	             (from its use as a numeral) the first: </strongs_def>
	            <kjv_def>--Alpha.</kjv_def> Often used (usually
	             <greek BETA="A)/N" unicode="ἄν" translit="án"/>, before a vowel) also in composition (as a contraction from <strongsref language="GREEK" strongs="427"/>) in
	             the sense of privation; so, in many words, beginning with this letter;
	             occasionally in the sense of union (as a contraction of <strongsref language="GREEK" strongs="260"/>).
	            <see language="GREEK" strongs="427"/>
	            <see language="GREEK" strongs="260"/>
	        </entry>
	        <entry strongs="02244">
	            <strongs>2244</strongs>
	            <greek BETA="H(LIKI/A" unicode="ἡλικία" translit="hēlikía"/>
	            <pronunciation strongs="hay-lik-ee'-ah"/>
	            <strongs_derivation>from the same as <strongsref language="GREEK" strongs="2245"/>;</strongs_derivation>
	            <strongs_def> maturity (in years or size)</strongs_def>
	            <kjv_def>:--age, stature.</kjv_def>
	            <see language="GREEK" strongs="2245"/>
	        </entry>
	    </entries>
	</strongsdictionary>

##2. Data Handling / Conversion

Found [these Perl scripts](http://code.google.com/p/osis-converters/) which might help for converting from USFM to OSIS.

There is a Javascript parsing library called [Bible Passage Reference Parser](https://github.com/openbibleinfo/Bible-Passage-Reference-Parser) which may be useful for us.

##3. Good source of OSIS bibles

* http://sourceforge.net/projects/zefania-sharp/
* http://biblos.com/