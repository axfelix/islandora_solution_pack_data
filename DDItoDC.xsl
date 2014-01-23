<xsl:stylesheet version="1.0"
		xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		xmlns:dc="http://purl.org/dc/terms/"
		xmlns:doc="http://www.icpsr.umich.edu/doc" 
		targetNamespace="http://www.icpsr.umich.edu/DDI"
		exclude-result-prefixes="dc">
<xsl:output method="xml" version="1.0" encoding="UTF-8"
		indent="yes" />
<xsl:template match="/">
<xsl:for-each select="codebook">
	<xsl:for-each select="stdyDscr">
		<xsl:for-each select="citation">
		<xsl:for-each select="titlStmt">
			<dc:title>
				<xsl:for-each select="titl">
					<xsl:value-of select="."/>
				</xsl:for-each>
			</dc:title>
			<dc:identifier>
				<xsl:for-each select="IDNo">
					<xsl:value-of select="."/>
				</xsl:for-each>
			</dc:identifier>
		</xsl:for-each>
		<xsl:for-each select="rspStmt">
			<dc:creator>
				<xsl:for-each select="AuthEnty">
					<xsl:value-of select="."/>
				</xsl:for-each>
			</dc:creator>
			<dc:contributor>
				<xsl:for-each select="othId">
					<xsl:value-of select="."/>
				</xsl:for-each>
			</dc:contributor>
		</xsl:for-each>
		<xsl:for-each select="prodStmt">
			<dc:publisher>
				<xsl:for-each select="producer">
					<xsl:value-of select="."/>
				</xsl:for-each>
			</dc:publisher>
			<dc:date>
				<xsl:for-each select="prodDate">
					<xsl:value-of select="."/>
				</xsl:for-each>
			</dc:date>
		</xsl:for-each>
		</xsl:for-each>
		<xsl:for-each select="stdyInfo">
			<dc:subject>
				<xsl:for-each select="subject/keyword">
					<xsl:value-of select="."/>
				</xsl:for-each>
			</dc:subject>
			<dc:description>
				<xsl:for-each select="abstract">
					<xsl:value-of select="."/>
				</xsl:for-each>
			</dc:description>
			<xsl:for-each select="sumDscr">
				<dc:coverage>
					<xsl:for-each select="geogCover">
						<xsl:value-of select="."/>
					</xsl:for-each>
				</dc:coverage>
				<dc:type>
					<xsl:for-each select="dataKind">
						<xsl:value-of select="."/>
					</xsl:for-each>
				</dc:type>
			</xsl:for-each>
		</xsl:for-each>
		<dc:source>
			<xsl:for-each select="method/dataColl/sources/dataSrc">
				<xsl:value-of select="."/>
			</xsl:for-each>
		</dc:source>
		<dc:rights>
			<xsl:for-each select="dataAccs/useStmt/restrctn">
				<xsl:value-of select="."/>
			</xsl:for-each>
		</dc:rights>
			<xsl:for-each select="otherStdyMat">
				<dc:relation>
					<xsl:for-each select="relMat">
						<xsl:value-of select="."/>
					</xsl:for-each>
				</dc:relation>
				<dc:isPartOf>
					<xsl:for-each select="relPubl"/>
						<xsl:value-of select="."/>
					</xsl:for-each>
				</dc:isPartOf>
			</xsl:for-each>
		</xsl:for-each>
	</xsl:for-each>
</xsl:template>
</xsl:stylesheet>