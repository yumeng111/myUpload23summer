<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" indent="yes"/>

  <xsl:param name="CFEBRXFILE"/>

  <xsl:template match="/">
    <xsl:apply-templates/>
  </xsl:template>

  <!-- Copy everything except text() since VME config XML tags have no text child nodes, only attributes -->
  <xsl:template match="*|@*|comment()|processing-instruction()">
    <xsl:copy >
      <xsl:apply-templates select="*|@*|comment()"/>
    </xsl:copy>
  </xsl:template>

  <!-- Change CFEB rx delays and posnegs when and where appropriate -->
  <xsl:template match="@cfeb0delay|@cfeb1delay|@cfeb2delay|@cfeb3delay|@cfeb4delay|@cfeb0123delay|@cfeb456delay|@cfeb0posneg|@cfeb1posneg|@cfeb2posneg|@cfeb3posneg|@cfeb4posneg|@cfeb0123posneg|@cfeb456posneg">
    <xsl:variable name="NAME"><xsl:value-of select="name(.)"/></xsl:variable>
    <xsl:variable name="CHAMBERLABEL"><xsl:value-of select="translate(../../@label,'0','')"/></xsl:variable>
    <xsl:choose>
      <xsl:when test="document($CFEBRXFILE)/scans/chamber[translate(@label,'0','')=$CHAMBERLABEL]/@*[name()=$NAME]">
	<xsl:message>
	  <xsl:text>Inserting for </xsl:text>
	  <xsl:value-of select="$CHAMBERLABEL"/>
	  <xsl:text> </xsl:text>
	  <xsl:value-of select="$NAME"/>
	  <xsl:text>=</xsl:text>
	  <xsl:value-of select="document($CFEBRXFILE)/scans/chamber[translate(@label,'0','')=$CHAMBERLABEL]/@*[name()=$NAME]"/>
	</xsl:message>
	<xsl:attribute name="{$NAME}"><xsl:value-of select="document($CFEBRXFILE)/scans/chamber[translate(@label,'0','')=$CHAMBERLABEL]/@*[name()=$NAME]"/></xsl:attribute>
      </xsl:when>
      <xsl:otherwise>
	<xsl:copy/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
