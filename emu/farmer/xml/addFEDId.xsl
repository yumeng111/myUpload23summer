<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" indent="yes"/>

<!-- Adds fedId attribute to DDU tags. Values are taken from RUI-to-computer_mapping.xml -->
<!-- Usage example: -->
<!-- xsltproc addFEDId.xsl RUI-to-chamber_mapping.xml_old > RUI-to-chamber_mapping.xml -->

  <xsl:template match="/">
    <xsl:apply-templates/>
  </xsl:template>

  <!-- Copy everything except text() since theer are no text child nodes, only attributes -->
  <xsl:template match="*|@*|comment()|processing-instruction()">
    <xsl:copy >
      <xsl:apply-templates select="*|@*|comment()"/>
    </xsl:copy>
  </xsl:template>

  
  <xsl:template match="DDU">
    <xsl:variable name="RUI_INSTANCE"><xsl:value-of select="../@instance"/></xsl:variable>
    <xsl:copy >
      <xsl:attribute name="fedId"><xsl:value-of select="document('RUI-to-computer_mapping.xml')//RUI[@instance=$RUI_INSTANCE]/@fedId"/></xsl:attribute>
      <xsl:apply-templates select="*|@*|comment()"/>
    </xsl:copy>
  </xsl:template>

</xsl:stylesheet>
