<xsl:stylesheet version="1.0"
		xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		xmlns:dc="http://purl.org/dc/terms/"
		xmlns:doc="http://www.icpsr.umich.edu/doc" 
		targetNamespace="http://www.icpsr.umich.edu/DDI"
		exclude-result-prefixes="dc">
<xsl:output method="xml" version="1.0" encoding="UTF-8"
		indent="yes" />
<xsl:template match="/">
<xsl:param name="prefix">ddi_</xsl:param>
<xsl:param name="suffix">_ms</xsl:param>
<xsl:for-each select="codebook">
	<xsl:for-each select="stdyDscr">
		<xsl:for-each select="citation">
		<xsl:for-each select="titlStmt">
			<xsl:for-each select="titl">
     			<field>
       				<xsl:attribute name="name">
       					<xsl:value-of select="concat($prefix, local-name(), $suffix)"/>
       				</xsl:attribute>
					<xsl:value-of select="."/>
				</field>
			</xsl:for-each>
			<xsl:for-each select="IDNo">
				<field>
					<xsl:attribute name="name">
        				<xsl:value-of select="concat($prefix, local-name(), '_', translate(@type, ' ', '_'), $suffix)"/>
      				</xsl:attribute>			
					<xsl:value-of select="."/>
				</field>
			</xsl:for-each>
		</xsl:for-each>
		<xsl:for-each select="rspStmt">
			<xsl:for-each select="AuthEnty">
				<field>
					<xsl:attribute name="name">
						<xsl:value-of select="concat($prefix, 'author', $suffix)"/>
					</xsl:attribute>
					<xsl:value-of select="."/>
				</field>
			</xsl:for-each>
			<xsl:for-each select="othId">
				<field>
					<xsl:attribute name="name">
						<xsl:value-of select="concat($prefix, 'contributor', $suffix)"/>
					</xsl:attribute>
					<xsl:value-of select="."/>
				</field>
			</xsl:for-each>
		</xsl:for-each>
		<xsl:for-each select="prodStmt">
			<xsl:for-each select="producer">
				<field>
					<xsl:attribute name="name">
						<xsl:value-of select="concat($prefix, local-name(), $suffix)"/>
					</xsl:attribute>
					<xsl:value-of select="."/>
				</field>
			</xsl:for-each>
			<xsl:for-each select="prodDate">
				<field>
					<xsl:attribute name="name">
						<xsl:value-of select="concat($prefix, local-name(), $suffix)"/>
					</xsl:attribute>
					<xsl:value-of select="."/>
				</field>
			</xsl:for-each>
		</xsl:for-each>
		</xsl:for-each>
		<xsl:for-each select="stdyInfo">
			<xsl:for-each select="subject/keyword">
				<field>
					<xsl:attribute name="name">
						<xsl:value-of select="concat($prefix, 'subject', $suffix)"/>
					</xsl:attribute>
					<xsl:value-of select="."/>
				</field>
			</xsl:for-each>
			<xsl:for-each select="abstract">
				<field>
					<xsl:attribute name="name">
						<xsl:value-of select="concat($prefix, local-name(), $suffix)"/>
					</xsl:attribute>
					<xsl:value-of select="."/>
				</field>
			</xsl:for-each>
			<xsl:for-each select="sumDscr">
				<xsl:for-each select="geogCover">
					<field>
						<xsl:attribute name="name">
							<xsl:value-of select="concat($prefix, local-name(), $suffix)"/>
						</xsl:attribute>
						<xsl:value-of select="."/>
					</field>
				</xsl:for-each>
				<xsl:for-each select="dataKind">
					<field>
						<xsl:attribute name="name">
							<xsl:value-of select="concat($prefix, 'resource_type', $suffix)"/>
						</xsl:attribute>
						<xsl:value-of select="."/>
					</field>
				</xsl:for-each>
			</xsl:for-each>
		</xsl:for-each>
			<xsl:for-each select="method/dataColl/sources/dataSrc">
				<field>
					<xsl:attribute name="name">
						<xsl:value-of select="concat($prefix, local-name(), $suffix)"/>
					</xsl:attribute>
					<xsl:value-of select="."/>
				</field>
			</xsl:for-each>
			<xsl:for-each select="dataAccs/useStmt/restrctn">
				<field>
					<xsl:attribute name="name">
						<xsl:value-of select="concat($prefix, local-name(), $suffix)"/>
					</xsl:attribute>
					<xsl:value-of select="."/>
				</field>
			</xsl:for-each>
			<xsl:for-each select="otherStdyMat/relPubl">
				<field>
					<xsl:attribute name="name">
						<xsl:value-of select="concat($prefix, 'host_title', $suffix)"/>
					</xsl:attribute>
					<xsl:value-of select="."/>
				</field>
			</xsl:for-each>
		</xsl:for-each>
	</xsl:for-each>
</xsl:template>
</xsl:stylesheet>